#include "common.h"

static void args_empty(void **state) {
    args *args = *state;

    assert_null(args->opts);
    assert_null(args->operands);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(args_empty),
    };

    return cmocka_run_group_tests(tests, common_setup, common_teardown);
}
