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

    const struct CMUnitTest args[] = {
        cmocka_unit_test(argparse_args_operand_string),
        cmocka_unit_test(argparse_args_operand_string_multiple),
        cmocka_unit_test(argparse_args_operand_number),
        cmocka_unit_test(argparse_args_operand_number_multiple),
        cmocka_unit_test(argparse_args_operand_mixed),
        cmocka_unit_test(argparse_args_mixed_opt_parse_with_disable_and_args_and_operands),
    };

    int ret = cmocka_run_group_tests_name("miscallenous", misc, NULL, NULL);
    ret += cmocka_run_group_tests_name("args", args, NULL, NULL);

    return ret;
}
