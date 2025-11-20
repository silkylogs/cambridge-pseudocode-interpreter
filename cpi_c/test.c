#include <assert.h>
#include "bless.h"

// -- Testing harness ----
//
// Allows the programmer to add tests, as well as execute them.
// The test function in question must have the signature `bool function_name(void);`
//
// Usage code:
// ```
// bool failing_test(void) { return 2 + 2 == 5; }
// bool succeeding_test(void) { return 1 + 1 == 2; }
// CP_ADD_TEST(failing_test);
// CP_ADD_TEST(succeeding_test);
// CP_RUN_TESTS();
// ```
//
// Sample output (stdout):
// ```
// main.c:76:1: Test: 0000000000/0000000001: "failing_test"... Error
// main.c:76:1: Test: 0000000001/0000000001: "succeeding_test"... Ok
// main.c:83:1: Summary: One or more tests failed
// ```

#define CP_LEVEL_LOG(LEVEL, PRINTF_FMT_STR, ...) \
	printf("%s:%d:1: " LEVEL ": " PRINTF_FMT_STR, __FILE__, __LINE__, __VA_ARGS__)
//#define CP_LOG(FMT_MSG, ...) CP_LEVEL_LOG("Log", FMT_MSG, __VA_ARGS__)
#define CP_WARN(FMT_MSG, ...) CP_LEVEL_LOG("Warning", FMT_MSG, __VA_ARGS__)

#define MAX_TESTS (0x80)
typedef struct Tests {
	bool (*(fns[MAX_TESTS]))(void);
	char *names[MAX_TESTS];
	int32_t ptr_to_top;
	int8_t padding[4];
} Tests;
static Tests g_tests = {
	.fns = {0},
	.names = {0},
	.ptr_to_top = -1,
};

#define CP_ADD_TEST(TEST_FN) cp_add_test(&g_tests, TEST_FN, #TEST_FN)
static void cp_add_test(Tests *ts, bool (*test_fn)(void), char *name) {
	ts->ptr_to_top++;

	if (ts->ptr_to_top >= MAX_TESTS) {
		CP_WARN(
			"The number of tests has overflowed the limit (%d) of the container.",
			ts->ptr_to_top
		);

		ts->ptr_to_top %= MAX_TESTS;
	}

	ts->fns[ts->ptr_to_top] = test_fn;
	ts->names[ts->ptr_to_top] = name;
}

#define CP_RUN_TESTS() cp_run_tests(g_tests)
static void cp_run_tests(Tests ts) {
	bool all_ok = true, ok;
	for (int32_t i = 0; i <= ts.ptr_to_top; ++i) {
		CP_LEVEL_LOG(
			"Test",
			"%5.5d/%5.5d: \"%s\"... %s\n",
			i, ts.ptr_to_top,
			ts.names[i],
			(ok = (ts.fns[i])())? "Ok" : "Error"
		);
		all_ok &= ok;
	}

	CP_LEVEL_LOG(
		"Summary", "%s\n",
		all_ok? "All tests passed" : "One or more tests failed"
	);
}

static bool test__char_in__general(void) {
	return char_in('0', "1234567890");
}

static bool test__char_in__absent(void) {
	return !char_in('a', "bcdefgh");
}

static bool test__char_in__vacuous_truth(void) {
	return char_in('a', "");
}

static bool test__count_longest_sequence_of_char_in_set__general(void) {
	char str[] = "0001a";
	size_t expected = 4;
	size_t got = count_longest_sequence_of_char_in_set(str, sizeof str, "01234567890");;
	printf("Expected %zu got %zu\n", expected, got);
	return expected == got;

}

static bool test__ascii_string_to_int32__general(void) {
	struct Int32Result result = {0};
	char str[] = "249"; char sz = sizeof str;
	int32_t expected = 249;

	result = ascii_string_to_int32(str, sz);
	printf("res: expected %d got %d\n", true, result.res);
	printf("val: expected %d got %d\n", expected, result.val);
	return (expected == result.val) && (result.res);
}

static bool test__ascii_string_to_int32__not_an_int(void) {
	struct Int32Result result = {0};
	result = ascii_string_to_int32("ahdfldsajfdaoiewfsdjfoj", 4);
	printf("res: expected %d got %d\n", 0, result.res);
	return (!result.res);
}

static bool test__ascii_string_to_int32__negative(void) {
	struct Int32Result result = {0};
	char str[] = "-249";
	char sz = sizeof str;
	int32_t expected = -249;

	result = ascii_string_to_int32(str, sz);
	printf("res: expected %d got %d\n", true, result.res);
	printf("val: expected %d got %d\n", expected, result.val);
	return (expected == result.val) && (result.res);
}

static bool test__try_parse_literal__integer_positive(void) {
	char str[] = "849034";
	char sz = sizeof str;
	struct Literal expected, got;
	bool res;

	expected.kind = ADT_INTEGER;
	expected.val.as_int32 = 849034;
	expected.is_valid = true;

	got = try_parse_literal(str, sz);
	printf("Expected: kind=%d, val.as_int32=%d, is_valid=%d\n", expected.kind, expected.val.as_int32, expected.is_valid);
	res = literal_shallow_equality(expected, got);
	printf("Got: kind=%d, val.as_int32=%d, is_valid=%d\n", got.kind, got.val.as_int32, got.is_valid);

	return res;
}

static bool test__try_parse_literal__integer_negative(void) {
	char str[] = "-849034";
	char sz = sizeof str;
	struct Literal expected, got;
	bool res;

	expected.kind = ADT_INTEGER;
	expected.val.as_int32 = -849034;
	expected.is_valid = true;

	got = try_parse_literal(str, sz);
	printf("Expected: kind=%d, val.as_int32=%d, is_valid=%d\n", expected.kind, expected.val.as_int32, expected.is_valid);
	res = literal_shallow_equality(expected, got);
	printf("Got: kind=%d, val.as_int32=%d, is_valid=%d\n", got.kind, got.val.as_int32, got.is_valid);

	return res;
}

static bool test__try_parse_literal__real_postive(void) {
	return false;
}

static bool test__try_parse_literal__real_negative(void) {
	return false;
}

static bool test__try_parse_literal__char_valid(void) {
	return false;
}

static bool test__try_parse_literal__char_empty(void) {
	return false;
}

static bool test__try_parse_literal__char_not_in_character_set(void) {
	return false;
}

static bool test__try_parse_literal__char_multi_character(void) {
	return false;
}

static bool test__try_parse_literal__string(void) {
	return false;
}

static bool test__try_parse_literal__string_empty(void) {
	return false;
}

static bool test__try_parse_literal__boolean(void) {
	return false;
}

static bool test__try_parse_literal__date(void) {
	return false;
}


static bool test__get_statement__VAR_DECL(void) {
	struct StatementTypeAndBounds expected, got;
	char stmt[] = "DECLARE x : INTEGER";

	expected.type = STMT_VAR_DECL;
	expected.start = stmt;
	expected.size = sizeof stmt - 1;

	got = get_statement(stmt);

	printf("Expected: .type=%d, .start=%p, .size=%zu\n", expected.type, expected.start, expected.size);
	printf("Got:      .type=%d, .start=%p, .size=%zu\n", got.type, got.start, got.size);

	return expected.type==got.type && expected.start==got.start && expected.size==got.size;
}

static bool test__get_statement__CONST_DECL(void) {
	struct StatementTypeAndBounds expected, got;
	char stmt[] = "CONSTANT x = 4324";

	expected.type = STMT_CONST_DECL;
	expected.start = stmt;
	expected.size = sizeof stmt - 1;

	got = get_statement(stmt);

	printf("Expected: .type=%d, .start=%p, .size=%zu\n", expected.type, expected.start, expected.size);
	printf("Got:      .type=%d, .start=%p, .size=%zu\n", got.type, got.start, got.size);

	return expected.type==got.type && expected.start==got.start && expected.size==got.size;
}
static bool test__get_statement__1D_ARRAY_DECL(void) {
	struct StatementTypeAndBounds expected, got;
	char stmt[] = "DECLARE StudentNames : ARRAY[1:30] OF STRING";

	expected.type = STMT_1D_ARRAY_DECL;
	expected.start = stmt;
	expected.size = sizeof stmt - 1;

	got = get_statement(stmt);

	printf("Expected: .type=%d, .start=%p, .size=%zu\n", expected.type, expected.start, expected.size);
	printf("Got:      .type=%d, .start=%p, .size=%zu\n", got.type, got.start, got.size);

	return expected.type==got.type && expected.start==got.start && expected.size==got.size;
}

static bool test__get_statement__2D_ARRAY_DECL(void) {
	struct StatementTypeAndBounds expected, got;
	char stmt[] = "DECLARE NoughtsAndCrosses : ARRAY[1:3,1:3] OF CHAR";

	expected.type = STMT_2D_ARRAY_DECL;
	expected.start = stmt;
	expected.size = sizeof stmt - 1;

	got = get_statement(stmt);

	printf("Expected: .type=%d, .start=%p, .size=%zu\n", expected.type, expected.start, expected.size);
	printf("Got:      .type=%d, .start=%p, .size=%zu\n", got.type, got.start, got.size);

	return expected.type==got.type && expected.start==got.start && expected.size==got.size;
}

// static bool test__detect_thing__comment(void) {
// 	char str[] = "// This is a comment\n"
// 	struct Thing thing = detect_thing(str, sizeof str);
// 	return	thing.kind == THING_KIND_COMMENT
// 		&&	(0 == strcmp("This is a comment", thing.as_comment.text));
// }

int main(void) {
	CP_ADD_TEST(test__char_in__general);
	CP_ADD_TEST(test__char_in__absent);
	CP_ADD_TEST(test__char_in__vacuous_truth);

	CP_ADD_TEST(test__count_longest_sequence_of_char_in_set__general);

	CP_ADD_TEST(test__ascii_string_to_int32__general);
	CP_ADD_TEST(test__ascii_string_to_int32__not_an_int);
	CP_ADD_TEST(test__ascii_string_to_int32__negative);
	

	CP_ADD_TEST(test__try_parse_literal__integer_positive);
	CP_ADD_TEST(test__try_parse_literal__integer_negative);
	CP_ADD_TEST(test__try_parse_literal__real_postive);
	CP_ADD_TEST(test__try_parse_literal__real_negative);
	CP_ADD_TEST(test__try_parse_literal__char_valid);
	CP_ADD_TEST(test__try_parse_literal__char_empty);
	CP_ADD_TEST(test__try_parse_literal__char_not_in_character_set);
	CP_ADD_TEST(test__try_parse_literal__char_multi_character);
	CP_ADD_TEST(test__try_parse_literal__string);
	CP_ADD_TEST(test__try_parse_literal__string_empty);
	CP_ADD_TEST(test__try_parse_literal__boolean);
	CP_ADD_TEST(test__try_parse_literal__date);

	CP_ADD_TEST(test__get_statement__VAR_DECL);
	CP_ADD_TEST(test__get_statement__CONST_DECL);
	CP_ADD_TEST(test__get_statement__1D_ARRAY_DECL);
	CP_ADD_TEST(test__get_statement__2D_ARRAY_DECL);

	CP_RUN_TESTS();

	return 0;
}
