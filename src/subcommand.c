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

    scmd->name = NULL;
    scmd->short_description = NULL;
    scmd->description = NULL;

    free(scmd);
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

    fprintf(stream, "%s:\n%s", scmd->name);

    if (scmd->args)
        return args_help(scmd->args, stream);

    return ARGPARSE_PASSED_NULL;
}
