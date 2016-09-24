#ifndef ARGPARSE_PRIVATE_H
#define ARGPARSE_PRIVATE_H

#include "logger.h"

#define LENGTH(array) sizeof(array) / sizeof(array[0])

/** Returns the maximum of padding of all options. */
size_t args_max_pad(const args *args);

#endif // ARGPARSE_PRIVATE_H
