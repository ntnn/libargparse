#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#define UNIT_TESTING 1
#include <cmocka.h>

#include "argparse.h"
#include "option.h"
#include "operand.h"
#include "parse.h"
#include "common.h"

static void option_empty() {
    option *opt = option_new("", "", "");

    assert_string_equal("", opt->short_opt);
    assert_string_equal("", opt->long_opt);
    assert_string_equal("", opt->description);
    assert_null(opt->argument);

    assert_false(opt->accepts_arguments);
    assert_false(opt->requires_arguments);

    assert_true(0 == opt->present);

    assert_null(opt->next);

    option_free(opt);
}

static void option_find_success() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    assert_non_null(option_find(args, "c"));

    args_free(args);
}

static void option_find_multiple() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    args_add_option(args, common_opt_destroy(""));

    assert_non_null(option_find(args, "destroy"));
    assert_non_null(option_find(args, "c"));

    args_free(args);
}

static void option_one_opt() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));

    assert_non_null(args->opts);
    assert_string_equal(
            "creation",
            args->opts->description
            );
    assert_string_equal("c", args->opts->short_opt);
    assert_string_equal("create", args->opts->long_opt);
    assert_null(args->opts->argument);
    assert_null(args->opts->next);

    args_free(args);
}

static void option_empty_opt_fail() {
    args *args = args_new();

    option *opt = option_new("", "", "");
    assert_true(EXIT_FAILURE == args_add_option(args, opt));

    args_free(args);
}

static void option_one_opt_with_arg() {
    args *args = args_new();
    option *opt = common_opt_create("");
    opt->argument = operand_new("argument");
    args_add_option(args, opt);

    assert_non_null(args->opts);
    assert_string_equal("c", args->opts->short_opt);
    assert_string_equal("create", args->opts->long_opt);
    assert_string_equal("argument", args->opts->argument->string);
    assert_null(args->opts->next);

    args_free(args);
}

static void option_multiple_options() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    args_add_option(args, common_opt_destroy(""));
    args_add_option(args, common_opt_zero(""));

    assert_non_null(args->opts);

    option *opt = args->opts;
    assert_string_equal(opt->short_opt, "c");
    assert_string_equal("create", opt->long_opt);
    assert_null(opt->argument);
    assert_non_null(opt->next);

    opt = opt->next;
    assert_string_equal(opt->short_opt, "d");
    assert_string_equal("destroy", opt->long_opt);
    assert_null(opt->argument);
    assert_non_null(opt->next);

    opt = opt->next;
    assert_string_equal(opt->short_opt, "0");
    assert_string_equal("zero", opt->long_opt);
    assert_null(opt->argument);
    assert_null(opt->next);

    args_free(args);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(option_empty),
        cmocka_unit_test(option_find_success),
        cmocka_unit_test(option_find_multiple),
        cmocka_unit_test(option_find_multiple_short_and_long),
        cmocka_unit_test(option_one_opt),
        cmocka_unit_test(option_empty_opt_fail),
        cmocka_unit_test(option_one_opt_with_arg),
        cmocka_unit_test(option_multiple_options),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
