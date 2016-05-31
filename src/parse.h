#ifndef ARGPARSE_PARSE_H
#define ARGPARSE_PARSE_H

#include "argparse.h"
#include "argparse_private.h"

#include "option.h"
#include "operand.h"

/** Parses an arbitrary number of short options in the form of '-abcde'.
 *
 *  \param args Args struct to find options in
 *  \param argument Pointer to string to parse
 *  \param option_argument Pointer to set a possible argument to, if the
 *         option allows or requires arguments
 *  \returns Pointer to option if it allows or requires arguments
 */
option *parse_short(args *args, const char *argument);

/** Parses a long option in the form of '--option(=argument)
 *
 *  \param args Args struct to find option in
 *  \param argument Pointer to string to parse
 *  \param option_argument Pointer to set a possible argument to, if the
 *         option allows or requires arguments
 *  \returns Pointer to option if it allows or requires arguments
 */
option *parse_long(args *args, const char *argument);

#endif // ARGPARSE_PARSE_H
