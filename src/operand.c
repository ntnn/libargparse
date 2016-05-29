#include "operand.h"

operand *operand_new(const char *string) {
    operand *operand = NULL;
    operand = malloc(sizeof(operand));

    operand->number = atoi(string);
    if (!operand->number) {
        // if operand->number is 0 atoi() failed because the passed
        // string is not a number
        operand->string = malloc(sizeof(char) * strlen(string));
        strcpy(operand->string, string);
    } else
        operand->string = NULL;

    operand->next = NULL;
    return operand;
}

void operand_free(operand *operand) {
    if (operand->next)
        operand_free(operand->next);

    free(operand->string);
    free(operand);
}

operand *operand_parse(
        const char *argument,
        const char argument_delimiter
        ) {
    if (!argument)
        return NULL;

    char *first_delim = strchr(argument, argument_delimiter);
    if (!first_delim) {
        // no delimiter found, parse argument into operand and return
        // pointer
        return operand_new(argument);
    }

    // calculate size of string between start of argument and delimiter
    // for char array on stack
    size_t size = (size_t)first_delim - (size_t)argument;

    operand *new = NULL;
    if (!size)
        new = operand_new("");
    else {
        char parsed[size];
        strncpy(parsed, argument, size);
        // make sure that the last byte is a nullbyte
        parsed[size] = '\0';

        new = operand_new(parsed);
    }

    // find next argument portion and set it as next
    first_delim = (char *)(size_t)first_delim + 1;
    new->next = operand_parse(first_delim, argument_delimiter);

    return new;
}
