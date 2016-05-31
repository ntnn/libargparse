#ifndef ARGPARSE_OPTION_H
#define ARGPARSE_OPTION_H

#include "argparse.h"
#include "argparse_private.h"

#include "operand.h"

/** Recursively frees an option struct.
 * Only useful if the option to be free'd is not related to an args
 * struct.
 *
 * \param opt option struct to free
 */
void option_free(option *opt);

/** Write help generated from the option struct to the passed stream.
 *
 *  \param opt option struct to generate help from
 *  \param stream stream to write help output to
 *  \return ARGPARSEcode
 */
ARGPARSEcode option_help(const option *opt, FILE *stream);
/** Add and argument to an option
 *
 *  \param opt option struct to add to option-argument to
 *  \param op operand struct to add to option
 *  \return ARGPARSEcode
 */
ARGPARSEcode option_add_argument(option *opt, operand *op);

#endif // ARGPARSE_OPTION_H
