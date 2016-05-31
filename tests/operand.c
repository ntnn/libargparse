#include "operand.h"
#include "common.h"

/** Uses operand_parse() to create a new operand list and checks that
 * the parsed operands are in the order specified in result.
 */
static void operand_wrapper(const char *string, char *result[], const char delimiter) {
    assert_non_null(string);
    assert_non_null(result);

    operand *op = operand_parse(string, delimiter);

    assert_non_null(result[0]);
    size_t len = sizeof(result) / sizeof(result[0]);

    operand *cur = op;
    for (size_t i = 0; i < len; ++i) {
        assert_non_null(result[i]);

        if (atoi(result[i])) {
            assert_true(cur->number == atoi(result[i]));
            assert_null(cur->string);
        } else {
            assert_true(cur->number == 0);
            assert_string_equal(cur->string, result[i]);
        }

        if (i + 1 < len) {
            assert_non_null(cur->next);
            cur = cur->next;
        }
    }

    operand_free(op);
}

static void operand_empty() {
    operand *operand = operand_new("");

    assert_string_equal("", operand->string);
    assert_true(0 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_string_parse() {
    char *result[] = { "string" };
    operand_wrapper("string", result, ':');
}

static void operand_string_parse_delim_at_end() {
    char *result[] = { "string", "" };
    operand_wrapper("string:", result, ':');
}

static void operand_string_parse_multiple_items() {
    char *result[] = {
        "string",
        "another",
        "more",
        "even"
    };
    operand_wrapper("string:another:more:even", result, ':');
}

static void operand_string_parse_empty_item() {
    char *result[] = { "string", "", "second" };
    operand_wrapper("string::second", result, ':');
}

static void operand_string_parse_different_delimiter() {
    char *result[] = { "string", "second" };
    operand_wrapper("string-second", result, '-');
}

static void operand_number_parse() {
    char *result[] = { "20" };
    operand_wrapper("20", result, ':');
}

static void operand_number_parse_multiple_items() {
    char *result[] = { "20", "40" };
    operand_wrapper("20:40", result, ':');
}

static void operand_number_parse_empty_item() {
    char *result[] = { "20", "", "40" };
    operand_wrapper("20::40", result, ':');
}

static void operand_number_parse_different_delimiter() {
    char *result[] = { "20", "40" };
    operand_wrapper("20-40", result, '-');
}

static void operand_number_parse_dash_delim_with_negative() {
    char *result[] = { "20", "", "40" };
    operand_wrapper("20--40", result, '-');
}

static void operand_number_max() {
    // maximum number specified by POSIX
    operand *operand = operand_new("2147483647");

    assert_null(operand->string);
    assert_true(2147483647 == operand->number);
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

static void operand_number_negative() {
    operand *operand = operand_new("-20");

    assert_null(operand->string);
    assert_true(-20 == operand->number);
    assert_null(operand->next);

    operand_free(operand);
}

static void operand_number_parse_negative() {
    char *result[] = { "-20" };
    operand_wrapper("-20", result, ':');
}

static void operand_mixed_parse() {
    char *result[] = { "20", "string", "-5002", "above_below" };
    operand_wrapper("20:string:-5002:above_below", result, ':');
}

int main() {
    const struct CMUnitTest operands[] = {
        cmocka_unit_test(operand_empty),
        cmocka_unit_test(operand_string_parse),
        cmocka_unit_test(operand_string_parse_delim_at_end),
        cmocka_unit_test(operand_string_parse_multiple_items),
        cmocka_unit_test(operand_string_parse_empty_item),
        cmocka_unit_test(operand_string_parse_different_delimiter),
        cmocka_unit_test(operand_number_parse),
        cmocka_unit_test(operand_number_parse_multiple_items),
        cmocka_unit_test(operand_number_parse_empty_item),
        cmocka_unit_test(operand_number_parse_different_delimiter),
        cmocka_unit_test(operand_number_parse_dash_delim_with_negative),
        cmocka_unit_test(operand_number_max),
        cmocka_unit_test(operand_number_negative_max),
        cmocka_unit_test(operand_number_negative),
        cmocka_unit_test(operand_number_parse_negative),
        cmocka_unit_test(operand_mixed_parse),
    };

    return cmocka_run_group_tests_name("operands", operands, NULL, NULL);
}
