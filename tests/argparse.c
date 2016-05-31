#include "common.h"

static void args_empty(void **state) {
    args *args = *state;

    assert_null(args->opts);
    assert_null(args->operands);
}

static void add_option_passed_null(void **state) {
    args *args = *state;

    assert_int_equal(
            ARGPARSE_PASSED_NULL,
            args_add_option(args, NULL)
            );
}

static void empty_option_fail(void **state) {
    args *args = *state;

    option *opt = option_new("", "", "");
    assert_int_equal(
            ARGPARSE_EMPTY_OPTION,
            args_add_option(args, opt)
            );

    option_free(opt);
}

static void add_operand_passed_null(void **state) {
    args *args = *state;

    assert_int_equal(
            ARGPARSE_PASSED_NULL,
            args_add_operand(args, NULL)
            );
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(args_empty),
        cmocka_unit_test(add_option_passed_null),
        cmocka_unit_test(empty_option_fail),
        cmocka_unit_test(add_operand_passed_null),
    };

    return cmocka_run_group_tests(tests, common_setup, common_teardown);
}
