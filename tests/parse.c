#include "common.h"

// for option_add_argument
#include "option.h"

/** Parse tests.
 *  argument_enable = true, yes, enable
 *  argument_disable = false, no, disable
 *  prefix_enable = enable, with
 *  prefix_disable = no, disable, without
 *
 *  [-|+]f                              short_opt, short_opt_negate
 *  [-|+]f argument                     short_opt_accepts, short_opt_requires
 *  [-|+]fargument                      short_opt_arg_no_space
 *  --feature                           long_opt
 *  --feature argument                  long_opt_accepts, long_opt_requires
 *  --feature=argument                  long_opt_arg_equals
 *
 *  --[prefix_mod-]feature              done with cpp dark magic
 *  [-|+]f argument_mod                 done with cpp dark magic
 *  --feature argument_mod              done with cpp dark magic
 *  --[prefix_mod-]feature argument_mod done with cpp dark magic
 */

static int parse_setup(void **state) {
    args **arguments = malloc(sizeof(args*) * 2);

    arguments[0] = args_new();
    if (!arguments[0]) {
        free(arguments);
        return EXIT_FAILURE;
    }

    arguments[1] = args_new();
    if (!arguments[1]) {
        free_args(arguments[0]);
        free(arguments);
        return EXIT_FAILURE;
    }

    *state = arguments;

    return EXIT_SUCCESS;
}

static int parse_teardown(void **state) {
    args **arguments = *state;

    args_free(arguments[0]);
    args_free(arguments[1]);

    free(*state);

    return EXIT_SUCCESS;
}

static void compare_operand(operand *expected, operand *parsed) {
    assert_non_null(expected);
    assert_non_null(parsed);

    if (expected->number) {
        assert_true(expected->number == parsed->number);
        assert_null(expected->string);
        assert_null(parsed->string);
    } else {
        assert_int_equal(0, parsed->number);

        assert_non_null(parsed->string);
        assert_string_equal(
                expected->string,
                parsed->string
                );
    }

    if (expected->next)
        compare_operand(expected->next, parsed->next);
}

static void compare_option(option *expected, option *parsed) {
    assert_non_null(expected);
    assert_non_null(parsed);

    assert_int_equal(expected->present, parsed->present);
    assert_int_equal(expected->accepts_arguments, parsed->accepts_arguments);
    assert_int_equal(expected->requires_arguments, parsed->requires_arguments);

    if (expected->argument) {
        assert_non_null(parsed->argument);

        compare_operand(
                expected->argument,
                parsed->argument
                );
    } else {
        // expected operand has no argument, make sure the
        // parsed operand didn't receive one either
        assert_null(parsed->argument);
    }
}

static void compare_args(args *expected, args *parsed) {
    assert_non_null(expected);
    assert_non_null(parsed);

    assert_int_equal(expected->operandsc, parsed->operandsc);

    if (expected->opts) {
        assert_non_null(parsed->opts);
        assert_ptr_not_equal(expected->opts, parsed->opts);

        option *curr = expected->opts;

        while (curr) {
            compare_option(
                    curr,
                    option_find(parsed, curr->long_opt)
                    );

            curr = curr->next;
        }
    }

    if (expected->operands) {
        assert_non_null(parsed->operands);
        assert_ptr_not_equal(expected->operands, parsed->operands);
        compare_operand(expected->operands, parsed->operands);
    }
}

/** Small wrapper to avoid duplicated code in wrapper() */
static void add_thing_to_args(args *args, void **things, ARGPARSEcode (*funcref)()) {
    assert_non_null(args);
    assert_non_null(funcref);

    if (!things)
        return;

    size_t len = LENGTH(things);
    for (size_t i = 0; i < len; ++i) {
        assert_int_equal(ARGPARSE_OK, (*funcref)(args, things[i]));
    }
}

/** Wrapper function that assigns the referring options and operands
 * lists to the expected and parsed args structs.
 *
 * Mainly for clarity, so the actual tests are readable.
 *
 * Arrays with the expected prefix are preset by the test functions and
 * contain the exact values the test expects to be parsed.
 *
 * Arrays with the parsed prefix are copies of the expected arrays sans
 * the preset values. These will be filled by args_parse() and then
 * matched against the expected values.
 */
static void wrapper(
        void **state,
        option **expected_options,
        operand **expected_operands,
        option **parsed_options,
        operand **parsed_operands,
        const char **arguments,
        const size_t argumentsc
        ) {
    args **states = *state;
    args *expected = states[0];
    args *parsed = states[1];

    add_thing_to_args(expected, (void **)expected_options, args_add_option);
    add_thing_to_args(expected, (void **)expected_operands, args_add_operand);
    add_thing_to_args(parsed, (void **)parsed_options, args_add_option);
    add_thing_to_args(parsed, (void **)parsed_operands, args_add_operand);

    args_parse(
            parsed,
            argumentsc,
            arguments
            );
    compare_args(expected, parsed);
}

/** Macro to define two arrays at once without repetition.
 *  TWO_ARRAYS(option, option_one, option_two, ...) result in:
 *
 *  type *expected_options[] = { option_one, option_two, ...};
 *  type *parsed_options[] = { option_one, option_two, ...};
 *
 *  Within the tests the expected_##type##s arrays are then used to
 *  define values that are expected to be parsed.
 */
#define TWO_ARRAYS(type, ...) type *expected_##type##s[] = { __VA_ARGS__ }; type *parsed_##type##s[] = { __VA_ARGS__ };

#define OPT(length, value, accept_or_require, postfix, ...) \
    static void length##_opt##postfix(void **state) { \
        TWO_ARRAYS(option, \
                common_opt_create(accept_or_require) \
                ); \
        \
        const char *arguments[] = { __VA_ARGS__ }; \
        expected_options[0]->present = value; \
        if (LENGTH(arguments) > 1) \
            assert_int_equal( \
                    ARGPARSE_OK, \
                    option_add_argument(expected_options[0], operand_new(arguments[1])) \
                    ); \
        \
        wrapper(state, \
                expected_options, \
                NULL, \
                parsed_options, \
                NULL, \
                arguments, \
                LENGTH(arguments) \
                ); \
    }

// short_opt
OPT(short, 1, "", , "-c")
// short_opt_negate
OPT(short, -1, "", _negate, "+c")
// long_opt
OPT(long, 1, "", , "--create")

// short_opt_accepts
OPT(short, 1, "accept", _accepts, "-c", "argument")
// long_opt_accepts
OPT(long, 1, "accept", _accepts, "--create", "argument")

// short_opt_requires
OPT(short, 1, "require", _requires, "-c", "argument")
// long_opt_requires
OPT(long, 1, "require", _requires, "--create", "argument")

static void short_opt_arg_no_space(void **state) {
    TWO_ARRAYS(option,
            common_opt_create("accept"),
            );

    expected_options[0]->present = 1;
    assert_int_equal(
            ARGPARSE_OK,
            option_add_argument(expected_options[0], operand_new("argument"))
            );

    const char *arguments[] = { "-cargument" };

    wrapper(state, expected_options, NULL, parsed_options, NULL, arguments, 1);
}

static void long_opt_arg_equals(void **state) {
    TWO_ARRAYS(option,
            common_opt_create("accept"),
            );

    expected_options[0]->present = 1;
    assert_int_equal(
            ARGPARSE_OK,
            option_add_argument(expected_options[0], operand_new("argument"))
            );
    const char *arguments[] = { "--create=argument" };

    wrapper(state, expected_options, NULL, parsed_options, NULL, arguments, 1);
}

 /** Preprocessor voodoo
  *  argument_enable = true, yes, enable
  *  argument_disable = false, no, disable
  *  prefix_enable = enable, with
  *  prefix_disable = no, disable, without
  */

#undef true
#undef false
/* #define CONCAT(a, b) a#b */

/** Generates functions in the pattern
 *      [short|long]_opt_mod_[argument_modifier]_[positive|negative]
 *       |                    |                   +- depends on the passed value
 *       |                    +- true, yes, enable, false, no , disable
 *       +- -c or --create
 */
#define OPT_MOD(length, arg, funcpostfix, prefix, modifier, value) \
        static void length##_opt_mod_##modifier##_##funcpostfix(void **state) { \
            LOG("argument: %s, modifier: %s, expected value: %d", prefix#arg, #modifier, value); \
            TWO_ARRAYS(option, common_opt_create("accept")); \
            expected_options[0]->present = value; \
            assert_int_equal( \
                    ARGPARSE_OK, \
                    option_add_argument(expected_options[0], operand_new(#modifier)) \
                    ); \
            const char *arguments[] = { prefix#arg, #modifier }; \
            wrapper(state, expected_options, NULL, parsed_options, NULL, arguments, 2); \
        }

#define BOTH_OPT_MOD(modifier, value) \
    OPT_MOD(short, c, positive, "-", modifier, value) \
    OPT_MOD(short, c, negative, "+", modifier, -value) \
    OPT_MOD(long, create, enable, "--enable-", modifier, value) \
    OPT_MOD(long, create, with, "--with-", modifier, value) \
    OPT_MOD(long, create, no, "--no-", modifier, -value) \
    OPT_MOD(long, create, disable, "--disable-", modifier, -value) \
    OPT_MOD(long, create, without, "--without=", modifier, -value)

BOTH_OPT_MOD(true, 1)
BOTH_OPT_MOD(yes, 1)
BOTH_OPT_MOD(enable, 1)

BOTH_OPT_MOD(false, -1)
BOTH_OPT_MOD(no, -1)
BOTH_OPT_MOD(disable, -1)

int main() {
    #define TEST(test) cmocka_unit_test_setup_teardown(test, parse_setup, parse_teardown)

    #define TEST_OPT_MOD(modifier) \
        TEST(short_opt_mod_##modifier##_positive), \
        TEST(short_opt_mod_##modifier##_negative), \
        TEST(long_opt_mod_##modifier##_enable), \
        TEST(long_opt_mod_##modifier##_with), \
        TEST(long_opt_mod_##modifier##_no), \
        TEST(long_opt_mod_##modifier##_disable), \
        TEST(long_opt_mod_##modifier##_without),

    const struct CMUnitTest tests[] = {

        TEST(short_opt),
        TEST(long_opt),

        TEST(short_opt_negate),

        TEST(short_opt_accepts),
        TEST(long_opt_accepts),

        TEST(short_opt_requires),
        TEST(long_opt_requires),

        TEST(short_opt_arg_no_space),
        TEST(long_opt_arg_equals),

        TEST_OPT_MOD(true)
        TEST_OPT_MOD(yes)
        TEST_OPT_MOD(enable)

        TEST_OPT_MOD(false)
        TEST_OPT_MOD(no)
        TEST_OPT_MOD(disable)

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
