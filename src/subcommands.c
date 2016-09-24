#include "subcommands.h"

subcommands *subcommands_new() {
    subcommands *scmds = NULL;
    if (!scmds)
        return NULL;

    scmds->args = NULL;
    scmds->count = 0;

    return scmds;
}

void subcommands_free(subcommands *scmds) {
    if (!scmds)
        return;

    if (scmds->scmd)
        subcommand_free(scmds->scmd);
    scmds->scmd = NULL;

    if (scmds->args)
        args_free(scmds->args);
    scmds->args = NULL;

    free(scmds);
}

ARGPARSEcode subcommands_add_scmd(subcommands *scmds, subcommand *scmd) {
    if (!scmds || !scmd)
        return ARGPARSE_PASSED_NULL;

    if (!scmds->scmd) {
        scmds->scmd = scmd;
        return ARGPARSE_OK;
    }

    subcommand *last = scmds->scmd;
    while (last->next != NULL)
        last = last->next;

    last->next = scmd;

    return ARGPARSE_OK;
}

ARGPARSEcode subcommands_parse(subcommands *scmds, size_t argc, char **argv) {
    if (!scmds)
        return ARGPARSE_PASSED_NULL;

    return ARGPARSE_OK;
}

ARGPARSEcode subcommands_help(const subcommands *scmds, FILE *stream) {
    if (!scmds || !stream)
        return ARGPARSE_PASSED_NULL;

    return ARGPARSE_OK;
}
