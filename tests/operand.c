#include "operand.h"
#include "common.h"

static void operand_empty() {
    operand *operand = operand_new("");

    assert_string_equal("", operand->string);
    assert_true(0 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_string() {
    operand *operand = operand_new("string");

    assert_string_equal("string", operand->string);
    assert_true(0 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_string_parse() {
    operand *operand = operand_parse("string", ':');

    assert_string_equal("string", operand->string);
    assert_true(0 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_string_parse_long() {
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

static void operand_string_parse_delim_at_end() {
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

static void operand_string_parse_multiple_items() {
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

static void operand_string_parse_multiple_delims() {
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

static void operand_string_parse_different_delimiter() {
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

static void operand_number() {
    operand *operand = operand_new("20");

    assert_null(operand->string);
    assert_true(20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_number_parse() {
    operand *operand = operand_parse("20", ':');

    assert_null(operand->string);
    assert_true(20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_number_parse_multiple_items() {
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

static void operand_number_parse_multiple_delims() {
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

static void operand_number_parse_different_delimiter() {
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

static void operand_number_parse_dash_delim_with_negative() {
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

static void operand_number_max() {
    // maximum number specified by POSIX
    operand *operand = operand_new("2147483647");

    assert_null(operand->string);
    assert_true(2147483647 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_number_negative() {
    operand *operand = operand_new("-20");

    assert_null(operand->string);
    assert_true(-20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_number_negative_max() {
    // minimum number specified by POSIX
    operand *operand = operand_new("-2147483647");

    assert_null(operand->string);
    assert_true(-2147483647 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_mixed_parse() {
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

int main() {
    const struct CMUnitTest operands[] = {
        cmocka_unit_test(operand_empty),
        cmocka_unit_test(operand_string),
        cmocka_unit_test(operand_string_parse),
        cmocka_unit_test(operand_string_parse_long),
        cmocka_unit_test(operand_string_parse_delim_at_end),
        cmocka_unit_test(operand_string_parse_multiple_items),
        cmocka_unit_test(operand_string_parse_multiple_delims),
        cmocka_unit_test(operand_string_parse_different_delimiter),
        cmocka_unit_test(operand_number),
        cmocka_unit_test(operand_number_parse),
        cmocka_unit_test(operand_number_parse_multiple_items),
        cmocka_unit_test(operand_number_parse_multiple_delims),
        cmocka_unit_test(operand_number_parse_different_delimiter),
        cmocka_unit_test(operand_number_parse_dash_delim_with_negative),
        cmocka_unit_test(operand_number_max),
        cmocka_unit_test(operand_number_negative),
        cmocka_unit_test(operand_number_negative_max),
        cmocka_unit_test(operand_mixed_parse),
    };

    return cmocka_run_group_tests_name("operands", operands, NULL, NULL);
}
