#include "subcommands.h"

subcommands *subcommands_new() {
    subcommands *scmds = NULL;
    scmds = malloc(sizeof(subcommands));

    if (!scmds)
        return NULL;

    scmds->args = NULL;
    scmds->count = 0;
    scmds->scmd = NULL;
    scmds->name = NULL;
    scmds->description = NULL;

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

    scmds->count++;

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
    if (!scmds || !argv)
        return ARGPARSE_PASSED_NULL;

    subcommand *scmd = scmds->scmd;
    while (scmd != NULL) {
        if (strcmp(scmd->name, *argv) == 0)
            return args_parse(scmd->args, argc--, argv++);

        scmd = scmd->next;
    }

    return ARGPARSE_NO_MATCH_FOUND;
}

ARGPARSEcode subcommands_help(const subcommands *scmds, FILE *stream) {
    if (!scmds || !stream)
        return ARGPARSE_PASSED_NULL;

    // TODO

    return ARGPARSE_OK;
}
