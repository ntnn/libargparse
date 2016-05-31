#ifndef ARGPARSE_LOGGER_H
#define ARGPARSE_LOGGER_H

#ifdef DEBUG

#include <stdio.h>
#define LOG(format, ...) fprintf(stderr, "%s:%s:%d:"format"\n", PACKAGE, __FILE__, __LINE__, __VA_ARGS__)

#else

#define LOG(format, ...)

#endif // DEBUG

#endif // ARGPARSE_LOGGER_H
