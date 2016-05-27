/**
 * The MIT License (MIT)
 * Copyright (c) 2016 Nelo-T. wallus <nelo@wallus.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef ARGPARSE_ARGPARSE_H
#define ARGPARSE_ARGPARSE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/** Struct representing an operand */
typedef struct {
    char *string; /** Passed string, if string cannot be converted to a number with atoi() */
    int number; /** Passed string converted to number, 0 if conversion fails */

    void *next; /** Pointer to next operand */
} operand;

/** Struct representing an option
 *  Each member is initialized as NULL, 0 or false
 *
 *  description, short_opt and long_opt should be string literals.
 */
typedef struct {
    char *description; /** Description for help output */
    char *short_opt; /** Short option, single char */
    char *long_opt; /** Long option, multiple chars */

    operand *argument; /** Argument for option, can be preset to a default */
    bool accepts_arguments; /** True if option accepts arguments */
    bool requires_arguments; /** True if option requires arguments, implies accepts_arguments */

    char argument_delimiter; /** Delimiter to split option-arguments on, the default is a colon */

    int present; /** Non-zero if flag was present, negative if prefixed with 'no-', 'disable-' or '+'  */

    void *next; /** Pointer to next option */
} option;

/** Struct representing arguments.
 * Each member is initialized as NULL or 0;
 */
typedef struct {
    option *opts; /** Options */

    size_t operandsc; /** Number of operands */
    operand *operands; /** Operands */
} args;

/** Initializer for args struct. */
args *args_new();

/** Recursively free args struct. */
void args_free(args *args);

/** Initializer for option struct */
option *option_new();

/** Add an option to args struct
 *
 *  \param args args structure to add option to
 *  \param opt option structure to add
 *  \returns EXIT_FAILURE if args or opt are NULL and if short_opt and
 *           long_opt are NULL
 *           EXIT_SUCCESS otherwise
 */
int args_add_opt(args *args, option *opt);

/** Find option by short or long option.
 *
 * The option has to be passed as a string, short options are considered
 * string of length 1, everything else is a long option.
 *
 * Example:
 * \code
 *  args *args = args_new();
 *  option *opt = option_new();
 *  opt->short_opt = "z";
 *  opt->long_opt = "zero";
 *  opt->accepts_arguments = 1;
 *  arg_add_opt(args, opt);
 *
 *  option *opt = option_find(args, "z");
 *  if (strcmp(opt->argument, "value") == 0)
 *       print("Passed value: %s", opt->argument);
 * \endcode
 *
 * \param args arguments
 * \param opt string identifying an option
 * \returns  option struct if option was found, NULL otherwise
 */
option *option_find(args *args, char *opt);

/** Parses arguments into an args struct.
 *
 *  \param args args struct
 *  \param argc count of elements in argv
 *  \param argv pointer to char array of options, option-argument and
 *              operands
 *  \returns EXIT_FAILURE if an error occured, such as:
 *           - an option which required an option-argument didn't
 *             receive one
 *           EXIT_SUCCESS otherwise
 */
int args_parse(args *args, size_t argc, char **argv);

/** Writes help generated from the args struct to the passed stream.
 *
 *  \param args args struct to generate help from
 *  \param stream stream to write to
 *  \returns EXIT_FAILURE if:
 *           - args or stream are NULL,
 *           - args contains no options
 *           - an error occurred during generating the help
 *           - an error occured during writing
 *           EXIT_SUCCESS otherwise
 */
int args_help(args *args, FILE *stream);

#endif // ARGPARSE_ARGPARSE_H
