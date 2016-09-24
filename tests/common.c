#include "common.h"

int common_setup(void **state) {
    args *args = args_new();
    if (!args)
        return EXIT_FAILURE;

    *state = args;

    return EXIT_SUCCESS;
}

int common_teardown(void **state) {
    args_free(*state);

    return EXIT_SUCCESS;
}

int scmd_setup(void **state) {
    subcommand *scmd = subcommand_new();
    if (!scmd)
        return EXIT_FAILURE;

    *state = scmd;

    return EXIT_SUCCESS;
}

int scmd_teardown(void **state) {
    subcommand_free(*state);

    return EXIT_SUCCESS;
}

option *common_option_wrapper(
        const char *shopt,
        const char *lopt,
        const char *descr,
        const char *accept_require
        ) {
    option *opt = option_new(shopt, lopt, descr);

    if (strncmp(accept_require, "accept", 6) == 0)
        opt->accepts_arguments = true;
    else if (strncmp(accept_require, "require", 7) == 0)
        opt->requires_arguments = true;

    return opt;
}

option *common_opt_create(const char *accept_require) {
    return common_option_wrapper("c", "create", "creation", accept_require);
}

option *common_opt_destroy(const char *accept_require) {
    return common_option_wrapper("d", "destroy", "destruction", accept_require);
}

option *common_opt_append(const char *accept_require) {
    return common_option_wrapper("a", "append", "appendix", accept_require);
}

option *common_opt_xor(const char *accept_require) {
    return common_option_wrapper("x", "xor", "xor'd", accept_require);
}

option *common_opt_zero(const char *accept_require) {
    return common_option_wrapper("0", "zero", "zero'd", accept_require);
}


