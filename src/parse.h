#ifndef ARGPARSE_PARSE_H
#define ARGPARSE_PARSE_H

#include "argparse.h"

/** Parses a series of short options in the form '-abcd' and sets the
 * passed present value for each of them.
 *
 *  \param args args struct containing the options
 *  \param passed pointer to char array to parse
 *  \param present value to set as `->present` in each option
 *  \returns a pointer to the last option, if it accepts or requires an
 *           argument
 */
option *parse_short_opts(args *args, char *passed, int present);

/** Parses a single argument into an operand.
 *
 *  \param args args struct to append operand to
 *  \param passed argument to parse into operand
 *  \returns EXIT_FAILURE if:
 *           - args or passed are NULL
 *           - memory allocation failed
 *           EXIT_SUCCESS otherwise
 */
int parse_operand(args *args, char *passed);

#endif // ARGPARSE_PARSE_H

