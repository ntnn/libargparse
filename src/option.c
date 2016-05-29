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

int option_help(option *opt, FILE *stream) {
    fprintf(stream, "  ");

    if (opt->short_opt)
        fprintf(stream, "-%s ", opt->short_opt);

    if (opt->long_opt)
        fprintf(stream, "--%s", opt->long_opt);

    if (opt->description)
        fprintf(stream, "\t%s", opt->description);

    printf("\n");
    return EXIT_SUCCESS;
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

int option_add_argument(option *opt, operand *op) {
    if (!opt || !op)
        return EXIT_FAILURE;

    operand *cur = opt->argument;
    if (!cur)
        opt->argument = op;
    else {
        while (cur->next)
            cur = cur->next;
        cur->next = op;
    }

    return EXIT_SUCCESS;
}
