#include "option.h"

option *option_new(
        const char *short_opt,
        const char *long_opt,
        const char *description
        ) {
    option *opt = NULL;

    opt = malloc(sizeof(option));

    if (!opt)
        return NULL;

    opt->short_opt = short_opt;
    opt->long_opt = long_opt;
    opt->description = description;

    opt->argument = NULL;
    opt->accepts_arguments = false;
    opt->requires_arguments = false;
    opt->argument_delimiter = ':';

    opt->present = 0;

    opt->next = NULL;

    return opt;
}

void option_free(option *opt) {
    if (opt->argument)
        operand_free(opt->argument);
    opt->argument = NULL;

    if (opt->next)
        option_free(opt->next);
    opt->next = NULL;

    free(opt);
}

ARGPARSEcode option_help(const option *opt, size_t padding, FILE *stream) {
    fprintf(stream, "  ");

    if (opt->short_opt)
        fprintf(stream, "-%s", opt->short_opt);
    else
        fprintf(stream, "  ");

    fprintf(stream, " ");

    if (opt->long_opt)
        fprintf(stream, "--%s", opt->long_opt);


    // TODO split long descriptions where appropriate and pad
    if (opt->description) {
        if (strlen(opt->long_opt) >= ARGPARSE_MAXLEN_LONG_OPT)
            fprintf(stream, "\n%*s", padding, " ");
        else
            fprintf(stream, "%*s", padding - option_padding(opt) + 2, " ");

        fprintf(stream, "%s", opt->description);
    }

    fprintf(stream, "\n");
    return ARGPARSE_OK;
}

size_t option_padding(option *opt) {
    // 5 is the magic number for the leading two spaces to indent, the
    // two characters representing the short option and the space
    // separating the short and the long option
    return 5 + strlen(opt->long_opt);
}

option *option_find(const args *args, const char *opt) {
    bool short_opt = false;
    char *passed;
    if (strlen(opt) == 1)
        short_opt = true;

    option *cur = args->opts;
    const char *compare;
    while (cur) {
        if (short_opt && cur->short_opt)
            compare = cur->short_opt;
        else if (cur->long_opt)
            compare = cur->long_opt;

        if (compare && strcmp(opt, compare) == 0)
            return cur;

        compare = NULL;
        cur = cur->next;
    }

    return NULL;
}

ARGPARSEcode option_add_argument(option *opt, operand *op) {
    if (!opt || !op)
        return ARGPARSE_PASSED_NULL;

    operand *cur = opt->argument;
    if (!cur) {
        opt->argument = op;

        // check for modifiers, since this is the first option-argument
        // to be added to the option
        char *modifiers[] = {
            "false",
            "no",
            "disable"
        };

        for (size_t i = 0; i < LENGTH(modifiers); ++i) {
            if (strcmp(op->string, modifiers[i]) == 0) {
                LOG("found modifier %s, flipping %d", modifiers[i], opt->present);
                opt->present *= -1;
                break;
            }
        }
    } else {
        while (cur->next)
            cur = cur->next;
        cur->next = op;
    }

    return ARGPARSE_OK;
}
