#include "common.h"
#include "subcommand.h"

static void subcommands_empty(void **state) {
    subcommands *scmds = *state;
    assert_non_null(scmds);
    assert_null(scmds->scmd);
    assert_null(scmds->args);
    assert_null(scmds->description);
    assert_int_equal(scmds->count, 0);
}

static void subcommands_add_scmd_empty(void **state) {
    subcommands *scmds = *state;

    assert_int_equal(
            ARGPARSE_PASSED_NULL,
            subcommands_add_scmd(scmds, NULL)
            );
}

static void subcommands_add_one_scmd(void **state) {
    subcommands *scmds = *state;
    subcommand *scmd = subcommand_new();

    assert_int_equal(
            ARGPARSE_OK,
            subcommands_add_scmd(scmds, scmd)
            );
    assert_ptr_equal(scmds->scmd, scmd);
}

static void subcommands_add_two_scmds(void **state) {
    subcommands *scmds = *state;
    subcommand *scmd_one = subcommand_new();
    subcommand *scmd_two = subcommand_new();

    assert_int_equal(
            ARGPARSE_OK,
            subcommands_add_scmd(scmds, scmd_one)
            );
    assert_int_equal(
            ARGPARSE_OK,
            subcommands_add_scmd(scmds, scmd_two)
            );

    assert_ptr_equal(scmds->scmd, scmd_one);
    assert_ptr_equal(scmds->scmd->next, scmd_two);
}

int main() {
    #define TEST(test) cmocka_unit_test_setup_teardown( \
            test, \
            scmds_setup, \
            scmds_teardown \
            )

    const struct CMUnitTest tests[] = {
        TEST(subcommands_empty),
        TEST(subcommands_add_scmd_empty),
        TEST(subcommands_add_one_scmd),
        TEST(subcommands_add_two_scmds),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
