#include <argparse.h>
#include <stdio.h>
#include <stdlib.h>

int main(size_t argc, char **argv) {
    args *args = args_new();
    if (!args) {
        fprintf(stderr, "argparse could not be initialized, aborting.\n");
        return EXIT_FAILURE;
    }

    option *opt = option_new("n", "", "do not output a trailing newline");
    if (!opt) {
        fprintf(stderr, "option could not be initialized, aborting.\n");
        return EXIT_FAILURE;
    }
    args_add_option(args, opt);

    // the pointer can be reused as args_free() takes care of
    // freeing everything correctly
    opt = option_new("e", "", "enable interpretation of backslash escapes");
    if (!opt) {
        fprintf(stderr, "option could not be initialized, aborting.\n");
        return EXIT_FAILURE;
    }
    args_add_option(args, opt);

    // pass command line arguments (without the leading $0) to let
    // argparse parse them
    args_parse(args, argc - 1, argv + sizeof(char));

    // option_find returns the option-struct which represents the flag,
    // the parameter can be the short as well as the long option
    opt = option_find(args, "e");
    bool interpret_escapes = false;
    if (opt->present > 0) {
        // The `> 0` is important since opt->present could be
        // negative, if an argument `+e` was passed.
        interpret_escapes = true;
        printf("enable interpretation of escapes\n");
    }

    bool trailing_newline = true;
    if (option_find(args, "n")->present > 0) {
        trailing_newline = false;
        printf("disable printing of a trialing newline\n");
    }

    // frees args and opts recursively, no need to keep track of
    // them
    args_free(args);

    return 0;
}
