#ifndef ARGPARSE_TESTS_COMMON_H
#define ARGPARSE_TESTS_COMMON_H

#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#define UNIT_TESTING 1
#include <cmocka.h>

#include "argparse.h"
#include "argparse_private.h"
#include "logger.h"

/** Setup function storing pointer to args struct in state */
int common_setup(void **state);
/** Teardown function freeing args struct pointed to by state */
int common_teardown(void **state);

/** Setup function storing pointer to scmd struct in state */
int scmd_setup(void **state);
/** Teardown function freeing scmd struct pointed to by state */
int scmd_teardown(void **state);

/** Setup function storing pointer to scmds struct in state */
int scmds_setup(void **state);
/** Teardown function freeing scmds struct pointed to by state */
int scmds_teardown(void **state);

/** Wrapper to create options */
option *common_option_wrapper(
        const char *shopt,
        const char *lopt,
        const char *descr,
        const char *accept_require
        );


/** "-c", "--create", "creation" */
option *common_opt_create(const char *accept_require);

/** "-d", "--destroy", "destruction" */
option *common_opt_destroy(const char *accept_require);

/** "-a", "--append", "appendix" */
option *common_opt_append(const char *accept_require);

/** "-x", "--xor", "xor'd" */
option *common_opt_xor(const char *accept_require);

/** "-z", "--zero", "zero'd" */
option *common_opt_zero(const char *accept_require);

#endif // ARGPARSE_TESTS_COMMON_H
