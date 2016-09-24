#ifndef ARGPARSE_SUBCOMMAND_H
#define ARGPARSE_SUBCOMMAND_H

#include "argparse.h"
#include "argparse_private.h"

/** Add operand to args struct attached to subcommand.
 * If no args struct is attached to the subcommand it will be created.
 *
 * Also see args_add_operand().
 */
ARGPARSEcode subcommand_add_operand(subcommand *scmd, operand *op);

#endif // ARGPARSE_SUBCOMMAND_H
