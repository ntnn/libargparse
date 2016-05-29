#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#define UNIT_TESTING 1
#include <cmocka.h>

#include "argparse.h"

static void args_empty() {
    args *args = args_new();

    assert_null(args->opts);
    assert_null(args->operands);

    args_free(args);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(args_empty),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
