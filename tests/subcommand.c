#include "common.h"
#include "subcommand.h"

static void subcommand_empty(void **state) {
    subcommand *scmd = *state;

    assert_non_null(scmd);
    assert_null(scmd->args);
    assert_null(scmd->name);
    assert_null(scmd->description);
    assert_null(scmd->next);
}

static void subcommand_add_option_no_args(void **state) {
    subcommand *scmd = *state;
    option *opt = common_opt_create("");

    assert_int_equal(
            ARGPARSE_OK,
            subcommand_add_option(scmd, opt)
            );
    assert_non_null(scmd->args);
    assert_ptr_equal(scmd->args->opts, opt);
}

static void subcommand_add_operand_no_args(void **state) {
    subcommand *scmd = *state;
    operand *op = operand_new("test");

    assert_int_equal(
            ARGPARSE_OK,
            subcommand_add_operand(scmd, op)
            );
    assert_ptr_equal(scmd->args->operands, op);
    assert_string_equal("test", scmd->args->operands->string);
}

int main() {
    #define TEST(test) cmocka_unit_test_setup_teardown( \
            test, \
            scmd_setup, \
            scmd_teardown \
            )

    const struct CMUnitTest tests[] = {
        TEST(subcommand_empty),
        TEST(subcommand_add_option_no_args),
        TEST(subcommand_add_operand_no_args),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
