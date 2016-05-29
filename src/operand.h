#ifndef ARGPARSE_OPERAND_H
#define ARGPARSE_OPERAND_H

#include "argparse.h"

/** Initializes an operand struct with a string.
 * You probably want to use operand_parse to slice the operand into
 * a single linked list.
 *
 * \param string string to parse into ->number or ->string
 * \returns pointer to operant struct
 */
operand *operand_new(const char *string);

/** Recursively frees an operand struct.
 * Unly useful if the operand to be free'd is not related to an option
 * or args struct.
 *
 * \param operand operand struct to free
 */
void operand_free(operand *operand);

/** Parse arguments sliced by argument_delimiter into a single linked
 * list.
 *
 * If you actually need to operate on operands you'll want to use this.
 *
 * \param argument string to parse
 * \param argument_delimiter delimiter to slice string with
 * \returns pointer to first item in list
 */
operand *operand_parse(
        const char *argument,
        const char argument_delimiter
        );

#endif // ARGPARSE_OPERAND_H
