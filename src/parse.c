#include "parse.h"

option *parse_short(args *args, const char *argument) {
    int present = 1;
    if (strncmp(argument, "+", 1) == 0)
        present = -1;

    /** argument = argument + sizeof(char); */
    argument++;

    for (size_t i = 0; i < strlen(argument); ++i) {
        // this is ugly
        char pass[1] = { argument[i] };
        option *opt = option_find(args, pass);

        if (opt) {
            opt->present = present;

            if (opt->requires_arguments || opt->accepts_arguments) {
                if (strlen(argument) > i + 1) {
                    // TODO check if opt only accepts arguments and if the
                    // next character are options
                    option_add_argument(
                            opt,
                            operand_parse(
                                argument + (sizeof(char) * i) + sizeof(char),
                                opt->argument_delimiter
                                )
                            );
                    return NULL;
                }

                return opt;
            }
        } else {
            // TODO error handling
            // option could not be found
            return NULL;
        }
    }
    return NULL;
}

option *parse_long(args *args, const char *argument) {
    argument = argument + strlen("--");
    size_t present = 1;

    char *prefixes[5] = {
        "no",
        "disable",
        "without",
        "enable",
        "with",
    };

    // index that separates present=1 and present=-1
    int sepindex = 3;

    for (int j = 0; j < sizeof(prefixes) / sizeof(char*); ++j) {
        char *prefix = prefixes[j];
        if (strncmp(argument,
                    prefix,
                    strlen(prefix)
                    ) == 0) {
            if (j >= sepindex)
                present = 1;
            else
                present = -1;
            // cut off prefix and trailing dash
            argument = argument + strlen(prefix) + strlen("-");

            break;
        }
    }

    option *opt = NULL;

    char *equals = strchr(argument, '=');
    if (equals) {
        // equals sign was found, argument is in the form
        // --(prefix-)feature=argument
        char *option_argument = equals + sizeof(char);

        size_t arglen = (size_t)equals - (size_t)argument;
        char *temp = malloc((equals - argument) * sizeof(char));

        strncpy(temp, argument, arglen);

        opt = option_find(args, temp);

        free(temp);

        if (!opt->accepts_arguments && !opt->requires_arguments) {
            // TODO error handling, detected equals sign even though the
            // option neither accepts nor requires and argument
            return NULL;
        }

        opt->present = present;
        option_add_argument(opt,
                operand_parse(option_argument, opt->argument_delimiter)
                );

        return NULL;
    }

    opt = option_find(args, argument);
    opt->present = present;

    if (opt->accepts_arguments || opt->requires_arguments)
        return opt;

    return NULL;
}

ARGPARSEcode args_parse(args *args, const size_t argc, const char **const argv) {
    if (!args || !argv)
        return ARGPARSE_PASSED_NULL;

    for (size_t i = 0; i < argc; ++i) {
        option *opt = NULL;
        const char *argument = argv[i];

        if (strcmp(argument, "--") == 0) {
            for (size_t j = i + 1; j < argc; ++j) {
                args_add_operand(
                        args,
                        operand_parse(argv[j], ' ')
                        );
            }

            return ARGPARSE_OK;
        } else if (strncmp(argument, "--", 2) == 0) {
            opt = parse_long(args, argument);
        } else if (
                strncmp(argument, "-", 1) == 0
                || strncmp(argument, "+", 1) == 0
                ) {
            opt = parse_short(args, argument);
        } else {
            args_add_operand(
                    args,
                    operand_parse(argv[i], ' ')
                    );
        }

        if (opt) {
            if (!opt->accepts_arguments && !opt->requires_arguments) {
                // opt was returned even though it doesn't accept nor
                // require arguments
                return ARGPARSE_FALSE_RETURN;
            }

            if (i + 1 >= argc) {
                if (opt->requires_arguments) {
                    // option requires argument but none is given
                    return ARGPARSE_ARG_REQUIRED;
                } else if (opt->accepts_arguments) {
                    // last option, option accepts arguments and
                    // none is given, return success
                    return ARGPARSE_OK;
                }
            }

            if (strncmp(argv[i + 1], "-", 1) == 0) {
                // next argument starts like an option
                if (opt->requires_arguments) {
                    // option requires argument but next is option
                    return ARGPARSE_ARG_REQUIRED;
                } else if (opt->accepts_arguments) {
                    // option accepts argument but none is given,
                    // continue
                    continue;
                }
            }

            opt->argument = operand_parse(argv[++i], opt->argument_delimiter);
        }
    }

    return ARGPARSE_OK;
}
