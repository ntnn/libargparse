#include "parse.h"

option *parse_short_opts(args *args, char *passed, int present) {
    // TODO check if passed is a long option

    for (size_t i = 0; i < strlen(passed); ++i) {
        // this is ugly
        char pass[0];
        pass[0] = passed[i];
        option *opt = option_find(args, pass);

        if (opt) {
            opt->present = present;
            if (opt->requires_arguments || opt->accepts_arguments)
                return opt;
        } else {
            // TODO error handling
        }
    }
    return NULL;
}

// TODO
// i don't like this, it's too close to operand_parse, especially since
// operand_parse is safer and could be called with space as
// argument_delimiter. maybe rewrite this as a wrapper for operand_parse
// (and with a different name)
int parse_operand(args *args, char *passed) {
    operand *new_op = operand_new(passed);
    if (!new_op)
        return EXIT_FAILURE;

    if (args->operands) {
        operand *cur = args->operands;
        while (cur->next)
            cur = cur->next;
        cur->next = new_op;
    } else
        args->operands = new_op;

    args->operandsc++;
    return EXIT_SUCCESS;
}

int args_parse(args *args, size_t argc, char **argv) {
    for (size_t i = 0; i < argc; ++i) {
        option *opt = NULL;
        if (strcmp(argv[i], "--") == 0) {
            // double dash is specified, only operands follow

            for (size_t j = i; j < argc; ++j)
                parse_operand(args, argv[j]);

            return EXIT_SUCCESS;

        } else if (strncmp(argv[i], "--", 2) == 0) {
            // double dash with more characters, long option
            size_t offset = 2;
            size_t present = 1;
            if (strncmp(argv[i], "--no-", 5) == 0) {
                present = -1;
                offset = 5;
            }

            if (strncmp(argv[i], "--disable-", 10) == 0) {
                present = -1;
                offset = 10;
            }

            // parse_long_opts
            opt = option_find(args, &argv[i][offset]);
            opt->present = present;
        } else if (argv[i][0] == '-' || argv[i][0] == '+') {

            int present = 1;
            if (argv[i][0] == '+')
                present = -1;

            // parse_short_opts returns a pointer to an option in if it
            // accepts an option. POSIX specifies that only the last
            // short option should require or accept an option-argument
            opt = parse_short_opts(args, &argv[i][1], present);
        } else {
            parse_operand(args, argv[i]);
        }

        if (opt
                && (opt->accepts_arguments || opt->requires_arguments)
               ) {
            if (opt->requires_arguments
                    && (i + 1 >= argc
                        || strcmp(argv[i + 1], "--") == 0))
                // option requires an argument but none is supplied
                // or the next argument is a double dash
                return EXIT_FAILURE;

            if (i + 1 < argc
                    && argv[i + 1][0] != '-') {
                opt->argument = operand_parse(
                        argv[i + 1],
                        opt->argument_delimiter
                        );
                ++i;
            }
        }
    }

    return EXIT_SUCCESS;
}
