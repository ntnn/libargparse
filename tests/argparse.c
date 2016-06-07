#include "common.h"
#include "option.h"

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
    #define TEST(test) cmocka_unit_test_setup_teardown( \
            test, \
            common_setup, \
            common_teardown \
            )

    const struct CMUnitTest tests[] = {
        TEST(args_empty),
        TEST(add_option_passed_null),
        TEST(empty_option_fail),
        TEST(add_operand_passed_null),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
