#include "option.h"

option *option_new() {
    option *opt = NULL;

    opt = malloc(sizeof(option));

    if (!opt)
        return NULL;

    opt->description = NULL;
    opt->short_opt = NULL;
    opt->long_opt = NULL;

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
