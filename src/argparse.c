#include "argparse.h"
#include "option.h"
#include "operand.h"

args *args_new() {
    args *args = NULL;

    args = malloc(sizeof(args));

    if (!args)
        return NULL;

    args->opts = NULL;
    args->operandsc = 0;
    args->operands = NULL;

    return args;
}

void args_free(args *args) {
    if (args->opts)
        option_free(args->opts);
    args->opts = NULL;

    if (args->operands)
        operand_free(args->operands);
    args->operands = NULL;

    free(args);
}

int args_add_option(args *args, option *opt) {
    if (!args || !opt)
        return EXIT_FAILURE;

    if (strcmp("", opt->short_opt) == 0
            && strcmp("", opt->long_opt) == 0)
        return EXIT_FAILURE;

    option *last = args->opts;
    if (last != NULL) {
        while (last->next != NULL)
            last = last->next;
        last->next = opt;
    } else
        args->opts = opt;

    return EXIT_SUCCESS;
}

int args_add_operand(args *args, operand *op) {
    if (!args || !op)
        return EXIT_FAILURE;

    operand *cur = args->operands;
    if (!cur)
        args->operands = op;
    else {
        while (cur->next)
            cur = cur->next;
        cur->next = op;
    }

    args->operandsc++;

    return EXIT_SUCCESS;
}

int args_help(const args *args, FILE *stream) {
    if (!args || !stream || !args->opts)
        return EXIT_FAILURE;

    option *opt = args->opts;
    while (opt) {
        option_help(opt, stream);
        opt = opt->next;
    }

    return EXIT_SUCCESS;
}
