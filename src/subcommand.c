#include "subcommand.h"

subcommand *subcommand_new() {
    subcommand *scmd = NULL;
    scmd = malloc(sizeof(subcommand));

    if (!scmd)
        return NULL;

    scmd->args = NULL;
    scmd->name = NULL;
    scmd->description = NULL;
    scmd->next = NULL;

    return scmd;
}

void subcommand_free(subcommand *scmd) {
    if (!scmd)
        return;

    if (scmd->args)
        args_free(scmd->args);
    scmd->args = NULL;

    if (scmd->next)
        subcommand_free(scmd->next);
    scmd->next = NULL;

    free(scmd);
}

ARGPARSEcode subcommand_attach_args(subcommand *scmd, args *args) {
    if (!args)
        return ARGPARSE_PASSED_NULL;

    if (scmd->args)
        return ARGPARSE_ARGS_EXIST;

    scmd->args = args;

    return ARGPARSE_OK;
}

ARGPARSEcode subcommand_add_option(subcommand *scmd, option *opt) {
    if (!scmd->args)
        scmd->args = args_new();

    return args_add_option(scmd->args, opt);
}

ARGPARSEcode subcommand_add_operand(subcommand *scmd, operand *op) {
    if (!scmd->args)
        scmd->args = args_new();

    return args_add_operand(scmd->args, op);
}

ARGPARSEcode subcommand_help(const subcommand *scmd, FILE *stream) {
    if (!scmd || !stream)
        return ARGPARSE_PASSED_NULL;

    // TODO
    return ARGPARSE_OK;
}
