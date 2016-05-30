#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#define UNIT_TESTING 1
#include <cmocka.h>

#include "argparse.h"
#include "parse.h"
#include "operand.h"
#include "parse.h"
#include "common.h"

static void parse_short_opt() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "-c" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present);

    args_free(args);
}

static void parse_short_opt_negate() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "+c" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present < 0);

    args_free(args);
}

static void parse_short_opt_accept_without_arg() {
    args *args = args_new();

    args_add_option(args, common_opt_create("accept"));
    const char *arguments[1] = { "-c" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present);

    args_free(args);
}

static void parse_short_opt_accept_with_arg() {
    args *args = args_new();

    args_add_option(args, common_opt_create("accept"));
    const char *arguments[2] = { "-c", "argument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 2, arguments));

    option *opt = option_find(args, "c");
    assert_true(opt->present);
    assert_string_equal("argument", opt->argument->string);

    args_free(args);
}

static void parse_short_opt_accept_with_arg_no_space() {
    args *args = args_new();

    args_add_option(args, common_opt_create("accept"));

    const char *arguments[1] = { "-cargument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));

    option *opt = option_find(args, "c");
    assert_true(opt->present);
    assert_non_null(opt->argument);
    assert_string_equal("argument", opt->argument->string);

    args_free(args);
}

static void parse_short_opt_require_without_arg() {
    args *args = args_new();

    args_add_option(args, common_opt_create("require"));
    const char *arguments[1] = { "-c" };

    assert_false(EXIT_SUCCESS == args_parse(args, 1, arguments));

    args_free(args);
}

static void parse_short_opt_require_with_arg() {
    args *args = args_new();

    args_add_option(args, common_opt_create("require"));

    const char *arguments[2] = { "-c", "argument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 2, arguments));

    assert_string_equal("argument",
            option_find(args, "c")->argument->string);

    args_free(args);
}

static void parse_short_opt_require_with_arg_no_space() {
    args *args = args_new();

    args_add_option(args, common_opt_create("require"));
    const char *arguments[1] = { "-cargument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_string_equal("argument",
            option_find(args, "c")->argument->string);

    args_free(args);
}

static void parse_short_opt_require_with_multiple_arg_no_space() {
    args *args = args_new();

    args_add_option(args, common_opt_create("require"));
    args_add_option(args, common_opt_zero(""));
    args_add_option(args, common_opt_destroy(""));
    const char *arguments[1] = { "-0cargument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));

    option *opt = option_find(args, "c");
    assert_true(opt->present);
    assert_string_equal("argument", opt->argument->string);

    opt = option_find(args, "zero");
    assert_true(opt->present);

    opt = option_find(args, "destroy");
    assert_false(opt->present);

    args_free(args);
}

static void parse_short_opt_require_with_double_dash() {
    args *args = args_new();

    args_add_option(args, common_opt_create("require"));

    const char *arguments[3] = { "-c", "--", "operand" };

    assert_true(EXIT_FAILURE == args_parse(args, 3, arguments));

    args_free(args);
}

static void parse_long_opt() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));

    const char *arguments[1] = { "--create" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present);

    args_free(args);
}

static void parse_long_opt_enable() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "--enable-create" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present > 0);

    args_free(args);
}

static void parse_long_opt_disable() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "--disable-create" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present < 0);

    args_free(args);
}

static void parse_long_opt_no() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "--no-create" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present < 0);

    args_free(args);
}

static void parse_long_opt_with() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "--with-create" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present > 0);

    args_free(args);
}

static void parse_long_opt_without() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    const char *arguments[1] = { "--without-create" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present < 0);

    args_free(args);
}

static void parse_long_opt_arg() {
    args *args = args_new();

    args_add_option(args, common_opt_create("accept"));
    const char *arguments[2] = { "--create", "argument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 2, arguments));

    option *opt = option_find(args, "create");
    assert_true(opt->present);
    assert_string_equal(opt->argument->string, "argument");

    args_free(args);
}

static void parse_long_opt_arg_with_equals() {
    args *args = args_new();

    args_add_option(args, common_opt_create("accept"));
    const char *arguments[1] = { "--create=argument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));

    option *opt = option_find(args, "create");
    assert_true(opt->present);
    assert_non_null(opt->argument);
    assert_string_equal(opt->argument->string, "argument");

    args_free(args);
}

static void parse_long_opt_arg_accept_with_double_dash() {
    args *args = args_new();

    args_add_option(args, common_opt_create("accept"));
    const char *arguments[] = { "--create", "--", "argument" };

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));

    option *opt = option_find(args, "create");
    assert_true(opt->present);

    assert_string_equal(args->operands->string, "argument");

    args_free(args);
}

static void parse_multiple_short_opt() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    args_add_option(args, common_opt_xor(""));
    args_add_option(args, common_opt_zero(""));
    args_add_option(args, common_opt_append(""));
    args_add_option(args, common_opt_destroy(""));

    const char *arguments[3] = { "-cx", "-a", "-0" };

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));
    assert_true(option_find(args, "c")->present);
    assert_true(option_find(args, "x")->present);
    assert_true(option_find(args, "0")->present);
    assert_true(option_find(args, "a")->present);
    assert_false(option_find(args, "d")->present);

    args_free(args);
}

static void parse_multiple_long_opt() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    args_add_option(args, common_opt_xor(""));
    args_add_option(args, common_opt_zero(""));
    args_add_option(args, common_opt_append(""));
    args_add_option(args, common_opt_destroy(""));

    const char *arguments[3] = { "--create", "--destroy", "--zero" };

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));
    assert_true(option_find(args, "create")->present);
    assert_false(option_find(args, "xor")->present);
    assert_true(option_find(args, "zero")->present);
    assert_false(option_find(args, "append")->present);
    assert_true(option_find(args, "destroy")->present);

    args_free(args);
}

static void parse_mixed_opt() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    args_add_option(args, common_opt_xor(""));
    args_add_option(args, common_opt_zero(""));
    args_add_option(args, common_opt_append(""));
    args_add_option(args, common_opt_destroy(""));

    const char *arguments[3] = { "-cx", "--append", "-0" };

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));
    assert_true(option_find(args, "create")->present);
    assert_true(option_find(args, "xor")->present);
    assert_true(option_find(args, "zero")->present);
    assert_true(option_find(args, "append")->present);
    assert_false(option_find(args, "destroy")->present);

    args_free(args);
}

static void parse_mixed_opt_with_operands() {
    args *args = args_new();

    args_add_option(args, common_opt_create(""));
    args_add_option(args, common_opt_xor(""));
    args_add_option(args, common_opt_zero("accept"));
    args_add_option(args, common_opt_append("accept"));
    args_add_option(args, common_opt_destroy(""));

    const char *arguments[7] = {
        "+cx",
        "--append",
        "an_argument",
        "-0",
        "some:thing:",
        "50",
        "operand",
    };

    assert_true(EXIT_SUCCESS == args_parse(args, 7, arguments));
    assert_true(option_find(args, "create")->present < 0);
    assert_true(option_find(args, "xor")->present < 0);

    option *zero = option_find(args, "zero");
    assert_true(zero->present);
    assert_non_null(zero->argument);
    assert_string_equal("some", zero->argument->string);

    assert_non_null(zero->argument->next);
    operand *next = zero->argument->next;
    assert_string_equal("thing", next->string);

    assert_non_null(next->next);
    next = next->next;
    assert_string_equal("", next->string);

    option *append = option_find(args, "append");
    assert_true(append->present);
    assert_non_null(append->argument);
    assert_string_equal("an_argument", append->argument->string);

    assert_false(option_find(args, "destroy")->present);

    operand *op = args->operands;
    assert_true(op->number == 50);

    op = op->next;
    assert_string_equal(op->string, "operand");
    assert_null(op->next);

    args_free(args);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(parse_short_opt),
        cmocka_unit_test(parse_short_opt_negate),
        cmocka_unit_test(parse_short_opt_accept_without_arg),
        cmocka_unit_test(parse_short_opt_accept_with_arg),
        cmocka_unit_test(parse_short_opt_accept_with_arg_no_space),
        cmocka_unit_test(parse_short_opt_require_without_arg),
        cmocka_unit_test(parse_short_opt_require_with_arg),
        cmocka_unit_test(parse_short_opt_require_with_arg_no_space),
        cmocka_unit_test(parse_short_opt_require_with_multiple_arg_no_space),
        cmocka_unit_test(parse_short_opt_require_with_double_dash),
        cmocka_unit_test(parse_long_opt),
        cmocka_unit_test(parse_long_opt_enable),
        cmocka_unit_test(parse_long_opt_disable),
        cmocka_unit_test(parse_long_opt_no),
        cmocka_unit_test(parse_long_opt_with),
        cmocka_unit_test(parse_long_opt_without),
        cmocka_unit_test(parse_long_opt_arg),
        cmocka_unit_test(parse_long_opt_arg_with_equals),
        cmocka_unit_test(parse_long_opt_arg_accept_with_double_dash),
        cmocka_unit_test(parse_multiple_short_opt),
        cmocka_unit_test(parse_multiple_long_opt),
        cmocka_unit_test(parse_mixed_opt),
        cmocka_unit_test(parse_mixed_opt_with_operands),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
