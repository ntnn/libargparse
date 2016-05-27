#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#define UNIT_TESTING 1
#include <cmocka.h>

#include <stdio.h>
#include <limits.h>

#include "argparse.h"

static void argparse_args_empty() {
    args *args = args_new();

    assert_null(args->opts);
    assert_null(args->operands);

    args_free(args);
}

static void argparse_option_empty() {
    option *opt = option_new();

    assert_null(opt->description);
    assert_null(opt->short_opt);
    assert_null(opt->long_opt);
    assert_null(opt->argument);

    assert_false(opt->accepts_arguments);
    assert_false(opt->requires_arguments);

    assert_true(0 == opt->present);

    assert_null(opt->next);

    option_free(opt);
}

void argparse_test_option_free_wrap(void *descr) {
    free(descr);
}

static void argparse_option_find() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    args_add_opt(args, opt);

    assert_true(opt == option_find(args, "c"));

    args_free(args);
}

static void argparse_option_find_multiple() {
    args *args = args_new();

    option *opt_c = option_new();
    opt_c->short_opt = "c";
    args_add_opt(args, opt_c);

    option *opt_d = option_new();
    opt_d->long_opt = "destruct";
    args_add_opt(args, opt_d);

    assert_true(opt_d == option_find(args, "destruct"));
    assert_true(opt_c == option_find(args, "c"));

    args_free(args);
}

static void argparse_option_find_multiple_short_and_long() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    args_add_opt(args, opt);

    option *opt2 = option_new();
    opt2->short_opt = "z";
    opt2->long_opt = "zero";
    args_add_opt(args, opt2);

    opt = option_new();
    opt->long_opt = "destruct";
    args_add_opt(args, opt);

    assert_true(opt2 == option_find(args, "zero"));
    assert_true(opt2 == option_find(args, "z"));

    args_free(args);
}

static void argparse_operand_string() {
    operand *operand = operand_new("string");

    assert_string_equal("string", operand->string);
    assert_true(0 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_string_parse() {
    operand *operand = operand_parse("string", ':');

    assert_string_equal("string", operand->string);
    assert_true(0 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_string_parse_long() {
    // this test is kind of pointless
    operand *first = operand_parse(
"string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string:string",
            ':');

    operand *op = first;
    for (int i = 1; i < 100; ++i) {
        assert_string_equal(op->string, "string");
        assert_true(0 == op->number);
        assert_non_null(op->next);
        op = op->next;
    }

    operand_free(first);
}

static void argparse_operand_string_parse_delim_at_end() {
    operand *first = operand_parse("string:", ':');

    assert_string_equal("string", first->string);
    assert_true(0 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("", second->string);
    assert_true(0 == second->number);
    assert_null(second->next);

    operand_free(first);
}

static void argparse_operand_string_parse_multiple_items() {
    operand *first = operand_parse("string:second_string", ':');

    assert_string_equal("string", first->string);
    assert_true(0 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("second_string", second->string);
    assert_true(0 == second->number);
    assert_null(second->next);

    operand_free(first);
}

static void argparse_operand_string_parse_multiple_delims() {
    operand *first = operand_parse("string::second_string", ':');

    assert_string_equal("string", first->string);
    assert_true(0 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("", second->string);
    assert_true(0 == second->number);
    assert_non_null(second->next);

    operand *third = second->next;
    assert_string_equal("second_string", third->string);
    assert_true(0 == third->number);
    assert_null(third->next);

    operand_free(first);
}

static void argparse_operand_string_parse_different_delimiter() {
    operand *first = operand_parse("string-second_string", '-');

    assert_string_equal("string", first->string);
    assert_true(0 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("second_string", second->string);
    assert_true(0 == second->number);
    assert_null(second->next);

    operand_free(first);
}

static void argparse_operand_number() {
    operand *operand = operand_new("20");

    assert_null(operand->string);
    assert_true(20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_number_parse() {
    operand *operand = operand_parse("20", ':');

    assert_null(operand->string);
    assert_true(20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_number_parse_multiple_items() {
    operand *first = operand_parse("20:40", ':');

    assert_null(first->string);
    assert_true(20 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_null(second->string);
    assert_true(40 == second->number);
    assert_null(second->next);

    operand_free(first);
}

static void argparse_operand_number_parse_multiple_delims() {
    operand *first = operand_parse("20::40", ':');

    assert_null(first->string);
    assert_true(20 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("", second->string);
    assert_true(0 == second->number);
    assert_non_null(second->next);

    operand *third = second->next;
    assert_null(third->string);
    assert_true(40 == third->number);
    assert_null(third->next);

    operand_free(first);
}

static void argparse_operand_number_parse_different_delimiter() {
    operand *first = operand_parse("20-40", '-');

    assert_null(first->string);
    assert_true(20 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_null(second->string);
    assert_true(40 == second->number);
    assert_null(second->next);

    operand_free(first);
}

static void argparse_operand_number_parse_dash_delim_with_negative() {
    operand *first = operand_parse("20--40", '-');

    assert_null(first->string);
    assert_true(20 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("", second->string);
    assert_true(0 == second->number);
    assert_non_null(second->next);

    operand *third = second->next;
    assert_null(third->string);
    assert_true(40 == third->number);
    assert_null(third->next);

    operand_free(first);
}

static void argparse_operand_number_max() {
    // maximum number specified by POSIX
    operand *operand = operand_new("2147483647");

    assert_null(operand->string);
    assert_true(2147483647 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_number_negative() {
    operand *operand = operand_new("-20");

    assert_null(operand->string);
    assert_true(-20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_number_negative_max() {
    // minimum number specified by POSIX
    operand *operand = operand_new("-2147483647");

    assert_null(operand->string);
    assert_true(-2147483647 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void argparse_operand_mixed_parse() {
    operand *first = operand_parse("20:string", ':');

    assert_null(first->string);
    assert_true(20 == first->number);
    assert_non_null(first->next);

    operand *second = first->next;
    assert_string_equal("string", second->string);
    assert_true(!second->number);
    assert_null(second->next);

    operand_free(first);
}

static void argparse_args_one_opt() {
    args *args = args_new();

    option *opt = option_new();
    opt->description = "A descriptive description";
    opt->short_opt = "f";
    opt->long_opt = "first";
    args_add_opt(args, opt);

    assert_non_null(args->opts);
    assert_string_equal(
            "A descriptive description",
            args->opts->description
            );
    assert_string_equal("f", args->opts->short_opt);
    assert_string_equal("first", args->opts->long_opt);
    assert_null(args->opts->argument);
    assert_null(args->opts->next);

    args_free(args);
}

static void argparse_args_empty_opt_fail() {
    args *args = args_new();

    option *opt = option_new();
    assert_true(EXIT_FAILURE == args_add_opt(args, opt));

    args_free(args);
}

static void argparse_args_one_opt_with_arg() {
    args *args = args_new();
    option *opt = option_new();
    opt->description = "A test option";
    opt->short_opt = "c";
    opt->long_opt = "create";
    opt->argument = operand_new("argument");
    args_add_opt(args, opt);

    assert_non_null(args->opts);
    assert_string_equal("c", args->opts->short_opt);
    assert_string_equal("create", args->opts->long_opt);
    assert_string_equal("argument", args->opts->argument->string);
    assert_null(args->opts->next);

    args_free(args);
}

static void argparse_args_multiple_options() {
    args *args = args_new();

    option *opt = option_new();
    opt->short_opt = "c";
    opt->long_opt =  "create";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "d";
    opt->long_opt = "destruct";
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "0";
    opt->long_opt = "zero";
    args_add_opt(args, opt);

    assert_non_null(args->opts);

    opt = args->opts;
    assert_string_equal(opt->short_opt, "c");
    assert_string_equal("create", opt->long_opt);
    assert_null(opt->argument);
    assert_non_null(opt->next);

    opt = opt->next;
    assert_string_equal(opt->short_opt, "d");
    assert_string_equal("destruct", opt->long_opt);
    assert_null(opt->argument);
    assert_non_null(opt->next);

    opt = opt->next;
    assert_string_equal(opt->short_opt, "0");
    assert_string_equal("zero", opt->long_opt);
    assert_null(opt->argument);
    assert_null(opt->next);

    args_free(args);
}

static void argparse_args_one_short_opt_parse() {
    args *args = args_new();

    option *opt = option_new();
    opt->description = "A test option";
    opt->short_opt = "c";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "-c";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "c")->present);

    args_free(args);
}

static void argparse_args_one_short_opt_parse_negate() {
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

static void argparse_args_one_short_opt_accept_without_arg_parse() {
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

static void argparse_args_one_short_opt_accept_with_arg_parse() {
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

static void argparse_args_one_short_opt_require_without_arg_parse() {
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

static void argparse_args_one_short_opt_require_with_arg_parse() {
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

static void argparse_args_one_short_opt_require_with_double_dash() {
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

static void argparse_args_one_long_opt_parse() {
    args *args = args_new();

    option *opt = option_new();
    opt->description = "A test option";
    opt->long_opt = "create";
    args_add_opt(args, opt);

    char *arguments[1];
    arguments[0] = "--create";

    assert_true(EXIT_SUCCESS == args_parse(args, 1, arguments));
    assert_true(option_find(args, "create")->present);

    args_free(args);
}

static void argparse_args_one_long_opt_parse_disable() {
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

static void argparse_args_one_long_opt_parse_no() {
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

static void argparse_args_multiple_short_opt_parse() {
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

static void argparse_args_multiple_long_opt_parse() {
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

static void argparse_args_mixed_opt_parse() {
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

static void argparse_args_operand_string() {
    args *args = args_new();

    char *arguments[1];
    arguments[0] = "operand";

    args_parse(args, 1, arguments);

    assert_null(args->opts);
    assert_string_equal(args->operands->string, "operand");

    args_free(args);
}

static void argparse_args_operand_string_multiple() {
    args *args = args_new();

    char *arguments[3];
    arguments[0] = "operand";
    arguments[1] = "another";
    arguments[2] = "and_more";

    args_parse(args, 3, arguments);

    assert_null(args->opts);

    operand *op = args->operands;
    assert_string_equal(op->string, "operand");

    op = op->next;
    assert_string_equal(op->string, "another");

    op = op->next;
    assert_string_equal(op->string, "and_more");

    assert_null(op->next);

    args_free(args);
}

static void argparse_args_operand_number() {
    args *args = args_new();

    char *arguments[1];
    arguments[0] = "42";

    args_parse(args, 1, arguments);

    assert_null(args->opts);
    assert_true(args->operands->number = 42);

    args_free(args);
}

static void argparse_args_operand_number_multiple() {
    args *args = args_new();

    char *arguments[3];
    arguments[0] = "42";
    arguments[1] = "8";
    arguments[2] = "40000";

    args_parse(args, 3, arguments);

    assert_null(args->opts);

    operand *op = args->operands;
    assert_true(op->number == 42);

    op = op->next;
    assert_true(op->number == 8);

    op = op->next;
    assert_true(op->number == 40000);

    assert_null(op->next);

    args_free(args);
}

static void argparse_args_operand_mixed() {
    args *args = args_new();

    char *arguments[2];
    arguments[0] = "operand";
    arguments[1] = "42";

    args_parse(args, 2, arguments);

    assert_null(args->opts);
    assert_string_equal(args->operands->string, "operand");

    operand *second = args->operands->next;
    assert_true(second->number = 42);

    args_free(args);
}

static void argparse_args_mixed_opt_parse_with_disable_and_args_and_operands() {
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
    opt->requires_arguments = true;
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "a";
    opt->long_opt = "append";
    opt->accepts_arguments = true;
    args_add_opt(args, opt);

    opt = option_new();
    opt->short_opt = "w";
    opt->long_opt = "width";
    args_add_opt(args, opt);

    char *arguments[7];
    arguments[0] = "+cx";
    arguments[1] = "--append";
    arguments[2] = "an_argument";
    arguments[3] = "-0";
    arguments[4] = "some:thing:";
    arguments[5] = "50";
    arguments[6] = "operand";

    assert_true(EXIT_SUCCESS == args_parse(args, 7, arguments));
    assert_true(option_find(args, "create")->present < 0);
    assert_true(option_find(args, "xor")->present < 0);

    option *zero = option_find(args, "zero");
    assert_true(zero->present);
    assert_string_equal("some", zero->argument->string);

    assert_non_null(zero->argument->next);
    operand *next = zero->argument->next;
    assert_string_equal("thing", next->string);

    assert_non_null(next->next);
    next = next->next;
    assert_string_equal("", next->string);

    option *append = option_find(args, "append");
    assert_true(append->present);
    assert_string_equal("an_argument", append->argument->string);

    assert_false(option_find(args, "width")->present);

    operand *op = args->operands;
    assert_true(op->number == 50);

    op = op->next;
    assert_string_equal(op->string, "operand");
    assert_null(op->next);

    args_free(args);
}

int main() {
    const struct CMUnitTest misc[] = {
        cmocka_unit_test(argparse_args_empty),
    };

    const struct CMUnitTest options[] = {
        cmocka_unit_test(argparse_option_empty),
        cmocka_unit_test(argparse_option_find),
        cmocka_unit_test(argparse_option_find_multiple),
        cmocka_unit_test(argparse_option_find_multiple_short_and_long),
    };

    const struct CMUnitTest operands[] = {
        cmocka_unit_test(argparse_operand_string),
        cmocka_unit_test(argparse_operand_string_parse),
        cmocka_unit_test(argparse_operand_string_parse_long),
        cmocka_unit_test(argparse_operand_string_parse_delim_at_end),
        cmocka_unit_test(argparse_operand_string_parse_multiple_items),
        cmocka_unit_test(argparse_operand_string_parse_multiple_delims),
        cmocka_unit_test(argparse_operand_string_parse_different_delimiter),
        cmocka_unit_test(argparse_operand_number),
        cmocka_unit_test(argparse_operand_number_parse),
        cmocka_unit_test(argparse_operand_number_parse_multiple_items),
        cmocka_unit_test(argparse_operand_number_parse_multiple_delims),
        cmocka_unit_test(argparse_operand_number_parse_different_delimiter),
        cmocka_unit_test(argparse_operand_number_parse_dash_delim_with_negative),
        cmocka_unit_test(argparse_operand_number_max),
        cmocka_unit_test(argparse_operand_number_negative),
        cmocka_unit_test(argparse_operand_number_negative_max),
        cmocka_unit_test(argparse_operand_mixed_parse),
    };

    const struct CMUnitTest args[] = {
        cmocka_unit_test(argparse_args_one_opt),
        cmocka_unit_test(argparse_args_empty_opt_fail),
        cmocka_unit_test(argparse_args_one_opt_with_arg),
        cmocka_unit_test(argparse_args_multiple_options),
        cmocka_unit_test(argparse_args_one_short_opt_parse),
        cmocka_unit_test(argparse_args_one_short_opt_parse_negate),
        cmocka_unit_test(argparse_args_one_short_opt_accept_without_arg_parse),
        cmocka_unit_test(argparse_args_one_short_opt_accept_with_arg_parse),
        cmocka_unit_test(argparse_args_one_short_opt_require_without_arg_parse),
        cmocka_unit_test(argparse_args_one_short_opt_require_with_arg_parse),
        cmocka_unit_test(argparse_args_one_short_opt_require_with_double_dash),
        cmocka_unit_test(argparse_args_one_long_opt_parse),
        cmocka_unit_test(argparse_args_one_long_opt_parse_disable),
        cmocka_unit_test(argparse_args_one_long_opt_parse_no),
        cmocka_unit_test(argparse_args_multiple_short_opt_parse),
        cmocka_unit_test(argparse_args_multiple_long_opt_parse),
        cmocka_unit_test(argparse_args_mixed_opt_parse),
        cmocka_unit_test(argparse_args_operand_string),
        cmocka_unit_test(argparse_args_operand_string_multiple),
        cmocka_unit_test(argparse_args_operand_number),
        cmocka_unit_test(argparse_args_operand_number_multiple),
        cmocka_unit_test(argparse_args_operand_mixed),
        cmocka_unit_test(argparse_args_mixed_opt_parse_with_disable_and_args_and_operands),
    };

    int ret = cmocka_run_group_tests_name("miscallenous", misc, NULL, NULL);
    ret += cmocka_run_group_tests_name("options", options, NULL, NULL);
    ret += cmocka_run_group_tests_name("operands", operands, NULL, NULL);
    ret += cmocka_run_group_tests_name("args", args, NULL, NULL);

    return ret;
}
