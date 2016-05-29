#ifndef ARGPARSE_OPTION_H
#define ARGPARSE_OPTION_H

#include "argparse.h"

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
 *  \return EXIT_FAILURE if:
 *          - opt or stream are NULL
 *          - opt does not contain description
 *          - error in fprintf
 *          EXIT_SUCCESS otherwise
 */
int option_help(const option *opt, FILE *stream);

#endif // ARGPARSE_OPTION_H
