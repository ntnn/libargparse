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
    const char *description; /** Description for help output */
    const char *short_opt; /** Short option, single char */
    const char *long_opt; /** Long option, multiple chars */

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

/** Struct representing a subcommand.
 *  Each member is initialized as NULL or 0.
 *
 *  The next pointer is necessary for subcommands_free() and
 *  subcommands_parse() to work correctly and also used to order the
 *  output for help().
 */
typedef struct {
    args *args; /** Arguments attached to subcommand */
    char *name; /** Name of subcommand */
    char *description; /** Description for help output */
    void *next; /** Pointer to next subcommand */
} subcommand;

/** Struct representing multiple subcommands
 * Utility struct used to keep a next-pointer out of the subcommand
 * struct.
 *
 * Additionally this allows for global arguments, e.g.:
 *  cmd --global-argument subcommand --scmd-argument
 */
typedef struct {
    subcommand *scmd;
    args *args;
    size_t count; /** Number of subcommands */
} subcommands;

/** Return codes for argparse functions.
 * String representations can be obtained via args_error()
 */
typedef enum {
    ARGPARSE_OK = 0,
    ARGPARSE_PASSED_NULL, /** Null pointer passed to function that requires a valid pointer */
    ARGPARSE_EMPTY_OPTION, /** Passed option's short_opt and long_opt are empty */
    ARGPARSE_FALSE_RETURN, /** An unexpected value was returned */
    ARGPARSE_ARG_REQUIRED, /** An option-argument is required but none was supplied */
    ARGPARSE_ARGS_EXIST, /** Trying to attach an args struct to a struct which already has an args struct */
    ARGPARSE_ARGS_EMPTY, /** Passed args struct is empty */
} ARGPARSEcode;

/** Initializer for args struct. */
args *args_new();

/** Recursively free args struct. */
void args_free(args *args);

/** Initializer for option struct
 *
 * \param short_opt Literal string with one char
 * \param long_opt Literal string with multiple chars
 * \param description Literal string describing what this option
 *        influences
 * \returns Pointer to the new option struct
 */
option *option_new(
        const char *short_opt,
        const char *long_opt,
        const char *description
        );

/** Add an option to args struct
 *
 *  \param args args structure to add option to
 *  \param opt option structure to add
 *  \returns ARGPARSEcode
 */
ARGPARSEcode args_add_option(args *args, option *opt);

/** Add an operand to args struct
 *
 *  \param args args structure to add operand to
 *  \param op operand structure to add
 *  \returns ARGPARSEcode
 */
ARGPARSEcode args_add_operand(args *args, operand *op);

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
option *option_find(const args *args, const char *opt);

/** Parses arguments into an args struct.
 *
 *  \param args args struct
 *  \param argc count of elements in argv
 *  \param argv pointer to char array of options, option-argument and
 *              operands
 *  \returns ARGPARSEcode
 */
ARGPARSEcode args_parse(args *args, size_t argc, char **argv);

/** Writes help generated from the args struct to the passed stream.
 *
 *  \param args args struct to generate help from
 *  \param stream stream to write to
 *  \returns ARGPARSEcode
 */
ARGPARSEcode args_help(const args *args, FILE *stream);

/** Initializer for subcommand struct.  */
subcommand *subcommand_new();

/** Recursively free subcommand struct. */
void subcommand_free(subcommand *scmd);

/** Attach an args struct to a subcommand
 * Can only be executed once, since each subcommand can only hold one
 * args struct at any time.
 *
 * \returns ARGPARSE_OK on success
 *          ARGPARSE_ARGS_EXIST an args struct is already attached
 *          ARGPARSE_EMPTY_ARGS passed args struct is empty
 */
ARGPARSEcode subcommand_attach_args(subcommand *scmd, args *args);

/** Add option to args struct attached to subcommand.
 * If no args struct is attached to the subcommand it will be created.
 *
 * Also see args_add_option().
 */
ARGPARSEcode subcommand_add_option(subcommand *scmd, option *opt);

/** Add operand to args struct attached to subcommand.
 * If no args struct is attached to the subcommand it will be created.
 *
 * Also see args_add_operand().
 */
ARGPARSEcode subcommand_add_operand(subcommand *scmd, operand *op);

ARGPARSEcode subcommand_help(const subcommand *scmd, FILE *stream);

subcommands *subcommands_new();

/** Add subcommand struct to subcommands. */
ARGPARSEcode subcommands_add_scmd(subcommands *subcommands, subcommand *scmd);

/** Add args struct to subcommands struct.
 * Can only be executed once, since each subcommands struct can only
 * hold one args struct at any time.
 *
 * \returns ARGPARSE_OK on success
 *          ARGPARSE_ARGS_EXIST an args struct is already attached
 *          ARGPARSE_EMPTY_ARGS passed args struct is empty
 */
ARGPARSEcode subcommands_add_args(subcommands *subcommands, args *args);

ARGPARSEcode subcommands_free(subcommands *subcommands);
ARGPARSEcode subcommands_parse(args *args, size_t argc, char **argv);
ARGPARSEcode subcommands_help(const subcommands *subcommands, FILE *stream);

#endif // ARGPARSE_ARGPARSE_H
