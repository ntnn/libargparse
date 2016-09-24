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

ARGPARSEcode args_add_option(args *args, option *opt) {
    if (!args || !opt)
        return ARGPARSE_PASSED_NULL;

    if (strcmp("", opt->short_opt) == 0
            && strcmp("", opt->long_opt) == 0)
        return ARGPARSE_EMPTY_OPTION;

    if (!args->opts) {
        args->opts = opt;
        return ARGPARSE_OK;
    }

    option *last = args->opts;
    while (last->next != NULL)
        last = last->next;
    last->next = opt;

    return ARGPARSE_OK;
}

ARGPARSEcode args_add_operand(args *args, operand *op) {
    if (!args || !op)
        return ARGPARSE_PASSED_NULL;

    operand *cur = args->operands;
    if (!cur)
        args->operands = op;
    else {
        while (cur->next)
            cur = cur->next;
        cur->next = op;
    }

    args->operandsc++;

    return ARGPARSE_OK;
}

ARGPARSEcode args_help(const args *args, FILE *stream) {
    if (!args || !stream)
        return ARGPARSE_PASSED_NULL;

    size_t padding = args_max_pad(args);
    option *opt = args->opts;
    while (opt) {
        ARGPARSEcode ret = option_help(opt, padding, stream);

        if (ret)
            return ret;

        opt = opt->next;
    }

    return ARGPARSE_OK;
}

size_t args_max_pad(const args *args) {
    size_t max_pad = 0;

    for (option *cur = args->opts; cur != NULL; cur = cur->next) {
        if (option_padding(cur) > max_pad)
            max_pad = option_padding(cur);
    }

    return max_pad;
}
