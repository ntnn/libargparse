#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#define UNIT_TESTING 1
#include <cmocka.h>

#include "argparse.h"
#include "parse.h"
#include "operand.h"
#include "parse.h"

static void parse_one_short_opt() {
    args *args = args_new();

    option *opt = option_new();
    opt->description = "A test parse";
    opt->short_opt = "c";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "-c";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present);

    args_free(args);
}

static void parse_one_short_opt_negate() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "+c";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present < 0);

    args_free(args);
}

static void parse_one_short_opt_accept_without_arg() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->accepts_arguments = true;
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "-c";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present);

    args_free(args);
}

static void parse_one_short_opt_accept_with_arg() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->accepts_arguments = true;
    args_add_opt(args, opt);

    char *arguments[2];
    arguments[0] = "-c";
    arguments[1] = "argument";

    assert_true(EXIT_SUCCESS == args_parse(args, 2, arguments));
    assert_true(option_find(args, "c")->present);
    assert_string_equal("argument", option_find(args, "c")->argument->string);

    args_free(args);
}

static void parse_one_short_opt_require_without_arg() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->requires_arguments = true;
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "-c";

    assert_false(EXIT_SUCCESS == args_parse(args, 1, arguments));

    args_free(args);
}

static void parse_one_short_opt_require_with_arg() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->requires_arguments = true;
    args_add_opt(args, opt);

    char *arguments[2];
    arguments[0] = "-c";
    arguments[1] = "argument";

    assert_true(EXIT_SUCCESS == args_parse(args, 2, arguments));
    assert_string_equal("argument", option_find(args, "c")->argument->string);

    args_free(args);
}

static void parse_one_short_opt_require_with_double_dash() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->requires_arguments = true;
    args_add_opt(args, opt);

    char *arguments[3];
    arguments[0] = "-c";
    arguments[1] = "--";
    arguments[2] = "operand";

    assert_true(EXIT_FAILURE == args_parse(args, 3, arguments));

    args_free(args);
}

static void parse_one_long_opt() {
    args *args = args_new();

    option *opt = option_new();
    opt->description = "A test parse";
    opt->long_opt = "create";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "--create";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present);

    args_free(args);
}

static void parse_one_long_opt_disable() {
    args *args = args_new();

    option *opt = option_new();
    opt->long_opt = "create";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "--disable-create";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present < 0);

    args_free(args);
}

static void parse_one_long_opt_no() {
    args *args = args_new();

    option *opt = option_new();
    opt->long_opt = "create";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "--no-create";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present < 0);

    args_free(args);
}

static void parse_multiple_short_opt() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "x";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "0";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "a";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "w";
    args_add_opt(args, opt);

    char *arguments[3];
    arguments[0] = "-cx";
    arguments[1] = "-a";
    arguments[2] = "-0";

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));
    assert_true(option_find(args, "c")->present);
    assert_true(option_find(args, "x")->present);
    assert_true(option_find(args, "0")->present);
    assert_true(option_find(args, "a")->present);
    assert_false(option_find(args, "w")->present);

    args_free(args);
}

static void parse_multiple_long_opt() {
    args *args = args_new();

    option *opt = option_new();
    opt->long_opt = "create";
    args_add_opt(args, opt);

    opt = option_new();
    opt->long_opt = "xor";
    args_add_opt(args, opt);

    opt = option_new();
    opt->long_opt = "zero";
    args_add_opt(args, opt);

    opt = option_new();
    opt->long_opt = "append";
    args_add_opt(args, opt);

    opt = option_new();
    opt->long_opt = "width";
    args_add_opt(args, opt);

    char *arguments[3];
    arguments[0] = "--create";
    arguments[1] = "--width";
    arguments[2] = "--zero";

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));
    assert_true(option_find(args, "create")->present);
    assert_false(option_find(args, "xor")->present);
    assert_true(option_find(args, "zero")->present);
    assert_false(option_find(args, "append")->present);
    assert_true(option_find(args, "width")->present);

    args_free(args);
}

static void parse_mixed_opt_parse() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->long_opt = "create";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "x";
    opt->long_opt = "xor";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "0";
    opt->long_opt = "zero";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "a";
    opt->long_opt = "append";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "w";
    opt->long_opt = "width";
    args_add_opt(args, opt);

    char *arguments[3];
    arguments[0] = "-cx";
    arguments[1] = "--append";
    arguments[2] = "-0";

    assert_true(EXIT_SUCCESS == args_parse(args, 3, arguments));
    assert_true(option_find(args, "create")->present);
    assert_true(option_find(args, "xor")->present);
    assert_true(option_find(args, "zero")->present);
    assert_true(option_find(args, "append")->present);
    assert_false(option_find(args, "width")->present);

    args_free(args);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(parse_one_short_opt),
        cmocka_unit_test(parse_one_short_opt_negate),
        cmocka_unit_test(parse_one_short_opt_accept_without_arg),
        cmocka_unit_test(parse_one_short_opt_accept_with_arg),
        cmocka_unit_test(parse_one_short_opt_require_without_arg),
        cmocka_unit_test(parse_one_short_opt_require_with_arg),
        cmocka_unit_test(parse_one_short_opt_require_with_double_dash),
        cmocka_unit_test(parse_one_long_opt),
        cmocka_unit_test(parse_one_long_opt_disable),
        cmocka_unit_test(parse_one_long_opt_no),
        cmocka_unit_test(parse_multiple_short_opt),
        cmocka_unit_test(parse_multiple_long_opt),
        cmocka_unit_test(parse_mixed_opt_parse),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
