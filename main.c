// -- Cambridge pseudocode interpeter ----
//
// The implementation for the Cambridge pseudocode programming langauge.
// Impl notes:
// - The C99 standard guarantees that `int`s are at least 16 bits wide.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

// -- Utility macros ----
//
// A set of macros intended to implement functionality in C a bit less tedious.

#define CP_LEVEL_LOG(LEVEL, PRINTF_FMT_STR, ...) \
    printf("%s:%d:1: " LEVEL ": " PRINTF_FMT_STR, __FILE__, __LINE__, __VA_ARGS__);

#define CP_LOG(FMT_MSG, ...) CP_LEVEL_LOG("Log", FMT_MSG, __VA_ARGS__);
#define CP_WARN(FMT_MSG, ...) CP_LEVEL_LOG("Warning", FMT_MSG, __VA_ARGS__);

static bool cp_assert_internal(bool b, const char *b_expression, const char *file, int line);
static bool cp_assert_internal(bool b, const char *b_expression, const char *file, int line) {
    if (!b)
    printf("%s:%d:1: Assertion failure:\n"
           "---------- Expression starts here: ----------\n"
           "%s\n"
           "---------- Expression ends here.   ----------\n\n",
           file, line, b_expression);
    return b;
}
#define CP_ASSERT(EXPR) cp_assert_internal((EXPR), #EXPR, __FILE__, __LINE__)

#define CP_ASSERT_SET(EXPR, BOOL)		\
    CP_ASSERT(EXPR);				\
    BOOL = (EXPR)

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

#define MAX_TESTS (0x100)
typedef struct Tests {
    bool (*(fns[MAX_TESTS]))(void);
    char *names[MAX_TESTS];
    int ptr_to_top;
    int8_t padding[4];
} Tests;
static Tests g_tests = {
    .fns = {0},
    .names = {0},
    .ptr_to_top = -1,
};

#define CP_ADD_TEST(TEST_FN) cp_add_test(&g_tests, TEST_FN, #TEST_FN);
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
    for (int i = 0; i <= ts.ptr_to_top; ++i) {
        CP_LEVEL_LOG(
            "Test",
            "%10.10d/%10.10d: \"%s\"... %s\n",
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

bool test_succeed(void) {
    return 1 + 1 == 2;
}

// -- Variable manager ---
//
// A vestigial module intended to manage variable data scheduled for removal.

struct CpVariableInfo {
	struct CpAsciiStringView name;
	struct CpVariableType type;
	// TODO: associate data?
};

#define CPVARIABLEMANAGER_VAR_STACK_SIZE ((ssize)(16))
struct CpVariableManager {
	ssize stack_idx;
	struct CpVariableInfo var_stack[CPVARIABLEMANAGER_VAR_STACK_SIZE];
};

void CpVariableManager_push_var_to_stack_unchecked(
	struct CpVariableManager *manager,
	struct CpAsciiStringView new_name,
	struct CpVariableType new_type);

void CpVariableManager_push_var_to_stack_unchecked(
	struct CpVariableManager *manager,
	struct CpAsciiStringView new_name,
	struct CpVariableType new_type)
{
	manager->var_stack[manager->stack_idx].name = new_name;
	manager->var_stack[manager->stack_idx].type = new_type;
	manager->stack_idx++;
}

bool CpVariableManager_push_var_to_stack_check(struct CpVariableManager *manager);
bool CpVariableManager_push_var_to_stack_check(struct CpVariableManager *manager)
{
	if (manager->stack_idx >= CPVARIABLEMANAGER_VAR_STACK_SIZE)
	return false;

	return true;
}

bool cp_test_CpVariableManager_push_var_to_stack_unchecked(void);
bool cp_test_CpVariableManager_push_var_to_stack_unchecked(void) {
	struct CpVariableManager manager = {0};
	bool all_ok = true;

	// Test general pushes
	struct CpAsciiStringView dummy_name = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("DummyVar");
	struct CpVariableType dummy_type = {
	.name = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("DummyType"),
	.id = 0,
	};
	CpVariableManager_push_var_to_stack_unchecked(&manager, dummy_name, dummy_type);

	ssize last_idx = manager.stack_idx - 1;
	all_ok &= CP_ASSERT(
	CpAsciiStringView_equals(
		manager.var_stack[last_idx].name,
		dummy_name));
	
	all_ok &= CP_ASSERT(
	0 == memcmp(
		&manager.var_stack[last_idx].type,
		&dummy_type,
		sizeof (struct CpVariableType)));
	
	
	return all_ok;
}

/*
bool cp_test_variable_management(void) {    
	char *declaration_tests[] = {
	"DECLARE Counter : INTEGER",
	"DECLARE TotalToPay : REAL",
	"DECLARE InputChar : CHAR",
	"DECLARE PlayerName: STRING",
	"DECLARE GameOver : BOOLEAN",
	"DECLARE SaveDate : DATE",
	};
	ssize len = sizeof declaration_tests / sizeof declaration_tests[0];
	
	for (ssize i = 0; i < len; ++i)
	printf("%d/%d: \"%s\"\n", i+1, len, declaration_tests[i]);
	return true;
}
*/

// -- Type manager ----
//
// A vestigial module intended to manage type data scheduled for removal.

#ifndef CP_SOURCE_TYPE_MANAGER
#define CP_SOURCE_TYPE_MANAGER

/*
// Memory
	struct CpUniquePtr {
	void *data;
	ssize data_len_bytes;
	bool is_valid;
	} data;

#define CPUNIQUEPTR_NULLPTR (struct CpUniquePtr){ .data=NULL; .data_len_bytes=0; .is_valid=false; }
// Memory
*/

	/* Indices double as the type ID */
struct CpTypeStack {
	char *backing_name_array;
	ssize backing_name_array_len;
	
	ssize *backing_name_size_array;
	ssize backing_name_size_array_len;
	
	ssize names_last_idx;
};

void CpTypeStack_init_with_size_unchecked(
	struct CpTypeStack *tstack,
	char *new_backing_name_array,
	ssize new_backing_name_array_len,
	ssize *new_backing_name_size_array,
	ssize new_backing_name_size_array_len);
void CpTypeStack_init_with_size_unchecked(
	struct CpTypeStack *tstack,
	char *new_backing_name_array,
	ssize new_backing_name_array_len,
	ssize *new_backing_name_size_array,
	ssize new_backing_name_size_array_len)
{
	tstack->backing_name_array_len = new_backing_name_array_len;
	tstack->backing_name_size_array_len = new_backing_name_size_array_len;
	
	tstack->backing_name_array = new_backing_name_array;
	tstack->backing_name_size_array = new_backing_name_size_array;
	
	tstack->names_last_idx = 0;
}


bool CpTypeStack_init_with_size_check(
	struct CpTypeStack *tstack,
	char *new_backing_name_array,
	ssize new_backing_name_array_len,
	ssize *new_backing_namelen_array,
	ssize  new_backing_namelen_array_len);

bool CpTypeStack_init_with_size_check(
	struct CpTypeStack *tstack,
	char *new_backing_name_array,
	ssize new_backing_name_array_len,
	ssize *new_backing_namelen_array,
	ssize  new_backing_namelen_array_len)
{
	bool is_tstack_ptr_nonnull, is_struct_zeroed,
	isvalid_new_backing_name_array,
	isvalid_new_backing_name_array_len,
	isvalid_new_backing_namelen_array,
	isvalid_new_backing_namelen_array_len;

	struct CpTypeStack zeroed_tstack;
	memset(&zeroed_tstack, 0, sizeof (struct CpTypeStack));

	is_tstack_ptr_nonnull = tstack != NULL;
	if (!is_tstack_ptr_nonnull) return false;

	is_struct_zeroed =
	0 == memcmp(&zeroed_tstack, tstack, sizeof (struct CpTypeStack));

	isvalid_new_backing_name_array = NULL != new_backing_name_array;
	isvalid_new_backing_name_array_len = new_backing_name_array_len > 0;
	
	isvalid_new_backing_namelen_array = NULL != new_backing_namelen_array;
	isvalid_new_backing_namelen_array_len = new_backing_namelen_array_len > 0;
	
	return is_tstack_ptr_nonnull
	&& is_struct_zeroed
	&& isvalid_new_backing_name_array
	&& isvalid_new_backing_name_array_len
	&& isvalid_new_backing_namelen_array
	&& isvalid_new_backing_namelen_array_len;
}

bool cp_test_CpTypeStack_init_with_size(void);
bool cp_test_CpTypeStack_init_with_size(void) {
	bool all_ok = true;

	/* Invalid sizes */ {
	struct CpTypeStack valid_stack;
	char valid_backing_name_array[0xff];
	ssize valid_backing_namelen_array[0xff];
	
	ssize invalid_size_negative = -23544325;
	ssize valid_size = 0xff;

	memset(&valid_stack, 0, sizeof (struct CpTypeStack));

	all_ok &= CP_ASSERT(
		!CpTypeStack_init_with_size_check(
		&valid_stack,
		valid_backing_name_array, invalid_size_negative,
		valid_backing_namelen_array, invalid_size_negative));

	}
	
	/* Non zeroed struct */ {
	struct CpTypeStack invalid_stack;
	char valid_backing_name_array[0xff];
	ssize valid_backing_namelen_array[0xff];
	ssize valid_size = 0xff;

	all_ok &= CP_ASSERT(
		!CpTypeStack_init_with_size_check(
		&invalid_stack,
		valid_backing_name_array, valid_size,
		valid_backing_namelen_array, valid_size));
	}
	
	/* Invalid pointers */ {
	struct CpTypeStack invalid_stack;
	char invalid_backing_name_array[0xff];
	ssize valid_backing_namelen_array[0xff];
	ssize valid_size = 0xff;

	all_ok &= CP_ASSERT(
		!CpTypeStack_init_with_size_check(
		&invalid_stack,
		invalid_backing_name_array, valid_size,
		valid_backing_namelen_array, valid_size));	
	}
	
	return all_ok;
}

bool cp_test_CpTypeManager(void);
bool cp_test_CpTypeManager(void) {
	bool all_ok = true;
	
	all_ok &= CP_ASSERT(cp_test_CpTypeStack_init_with_size());
	
	return all_ok;
}

#endif

// -- String view ----
//
// A vestigial module scheduled for removal.

#ifndef CP_SRC_ASCII_STRING_VIEW
#define CP_SRC_ASCII_STRING_VIEW

struct CpAsciiStringView {
	char *chars;
	ssize len_including_nul;
};

// -- from_const_cstr --------------------------

struct CpAsciiStringView CpAsciiStringView_from_const_cstr_unchecked(const char *cstr, ssize cstr_len);
bool CpAsciiStringView_from_const_cstr_check(const char *cstr, ssize cstr_len);
bool CpAsciiStringView_check_well_formed(struct CpAsciiStringView str);
bool cp_test_CpAsciiStringView_from_const_cstr(void);


struct CpAsciiStringView
CpAsciiStringView_from_const_cstr_unchecked(const char *cstr, ssize cstr_len) {
	return (struct CpAsciiStringView) {
		.chars = (char *)cstr,
		.len_including_nul = cstr_len,
	};
}

#define CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED(CSTR)		\
	CpAsciiStringView_from_const_cstr_unchecked((CSTR), sizeof (CSTR))


bool
CpAsciiStringView_from_const_cstr_check(const char *cstr, ssize cstr_len) {
	if (cstr == NULL) return false;
	if (cstr_len < 0) return false;
	return true;
}

#define CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(CSTR)			\
	CpAsciiStringView_from_const_cstr_check((CSTR), sizeof (CSTR))


// Note: This exists in the first place because I thought some casting may be required
//       in order to call const_cstr_check
bool CpAsciiStringView_check_well_formed(struct CpAsciiStringView str) {
	return CpAsciiStringView_from_const_cstr_check(str.chars, str.len_including_nul);
}


bool cp_test_CpAsciiStringView_from_const_cstr(void) {
	// Test the macro checks for obvious defects  
	CP_ASSERT(false == CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(NULL));
	CP_ASSERT(true == CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(""));

	struct CpAsciiStringView test;
	test = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("This string has 25 chars");
	CP_ASSERT(test.len_including_nul == 25);
	CP_ASSERT(test.chars != NULL);
  
	return true;
}

// -- from_const_cstr --------------------------


// -- substring --------------------------------

struct CpAsciiStringView CpAsciiStringView_substring_unchecked(
	struct CpAsciiStringView string,
	ssize idx1, ssize idx2);
bool CpAsciiStringView_substring_check(
	struct CpAsciiStringView string,
	ssize idx1, ssize idx2);


struct CpAsciiStringView
CpAsciiStringView_substring_unchecked(struct CpAsciiStringView string, ssize idx1, ssize idx2) {
	if (idx1 > idx2) { ssize temp = idx1; idx1 = idx2; idx2 = temp; }

	struct CpAsciiStringView substr = {
	.chars = &string.chars[idx1],
	.len_including_nul = idx2 - idx1
	};

	return substr;
}



bool
CpAsciiStringView_substring_check(struct CpAsciiStringView string, ssize idx1, ssize idx2) {
	// Out of bounds check
	if (idx1 < 0 || idx1 >= string.len_including_nul) return false;
	if (idx2 < 0 || idx2 >= string.len_including_nul) return false;

	// Source string anomaly check
	if (!CpAsciiStringView_check_well_formed(string)) return false;

	return true;
}

// -- substring --------------------------------


struct CpAsciiStringView_StringSubstringPair {
	struct CpAsciiStringView string;
	struct CpAsciiStringView substring;
};

bool CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(
	struct CpAsciiStringView_StringSubstringPair);

bool
CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(
	struct CpAsciiStringView_StringSubstringPair pair)
{
	struct CpAsciiStringView string = pair.string;
	struct CpAsciiStringView substring = pair.substring;
	
	if (substring.len_including_nul > string.len_including_nul) return false;
	for (ssize str_i = 0; str_i < string.len_including_nul; ++str_i) {
	if (string.chars[str_i] == substring.chars[0]) {
		for (ssize sub_i = 0;
		 sub_i < substring.len_including_nul && sub_i < substring.len_including_nul;
		 ++sub_i)
		{
		char str_char = string.chars[str_i + sub_i];
		char sub_char = substring.chars[sub_i];
		
		//printf("Comparing \'%c\' with \'%c\'\n", str_char, sub_char);
		if (sub_char == '\0') return true;
		if (str_char != sub_char) return false;
		}
	}
	}
	return true;
}

#define CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(...)	\
	CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked( \
	(struct CpAsciiStringView_StringSubstringPair) {		\
		__VA_ARGS__							\
	})

// Tests
bool
cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(void);
bool
cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(void) {
	bool all_ok = true;

	// Tests for intended usage
	all_ok &= CP_ASSERT(
		CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(
		.string = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("DECLARE TestVar: BOOL"),
		.substring = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("BOOL"))
	);

	all_ok &= CP_ASSERT(
		CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(
		.substring = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("INTEGER"),
		.string = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("INTEGER64"))
	);

	// Improper usage: substring longer than original string
	all_ok &= CP_ASSERT(
		!CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(
		.string = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED(
		"Obviously long test string"),
		.substring = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED(
		"Obviously longer test string"))
	);

	return all_ok;
}

bool CpAsciiStringView_equals(struct CpAsciiStringView a, struct CpAsciiStringView b);
bool CpAsciiStringView_equals(struct CpAsciiStringView a, struct CpAsciiStringView b) {
	if (a.len_including_nul != b.len_including_nul) return false;
	ssize len = a.len_including_nul;

	return 0 == memcmp(a.chars, b.chars, len);
}

#endif


// -- Lexer ----
//
// Converts unformatted string input into a stream of tokens.
// A token is defined as a representative of text in the source
// which belongs to a fixed set of values.

enum TokenKind {
    TOK_NULL=0,
    TOK_SINGLEQUOTE,    TOK_DOUBLEQUOTE,    TOK_LPAREN,     TOK_RPAREN,
    TOK_LBRACKET,       TOK_RBRACKET,       TOK_LBRACE,     TOK_RBRACE,

    TOK_MINUS,          TOK_LARROW,         TOK_STAR,       TOK_SLASH,
    TOK_2SLASH,         TOK_PLUS,           TOK_LT,         TOK_LEQ,

    TOK_NEQ,            TOK_EQ,             TOK_GT,         TOK_GEQ,
    TOK_AMPERSAND,      TOK_AND,            TOK_APPEND,     TOK_ARRAY,

    TOK_BOOLEAN,        TOK_BYREF,          TOK_BYVAL,      TOK_CALL,
    TOK_CASE,           TOK_OF,             TOK_CHAR,       TOK_CLOSEFILE,

    TOK_CONSTANT,       TOK_DATE,           TOK_DECLARE,    TOK_DIV,
    TOK_ELSE,           TOK_ENDCASE,        TOK_ENDFUNCTION,TOK_ENDIF,

    TOK_ENDPROCEDURE,   TOK_ENDTYPE,        TOK_EOF,        TOK_FALSE,
    TOK_FOR,            TOK_TO,             TOK_FUNCTION,   TOK_GETRECORD,

    TOK_IF,             TOK_INPUT,          TOK_INTEGER,    TOK_LCASE,
    TOK_LENGTH,         TOK_MID,            TOK_MOD,        TOK_NEXT,

    TOK_NOT,            TOK_OPENFILE,       TOK_OR,         TOK_OTHERWISE,
    TOK_OUTPUT,         TOK_PROCEDURE,      TOK_PUTRECORD,  TOK_READ,

    TOK_READFILE,       TOK_REAL,           TOK_REPEAT,     TOK_RETURN,
    TOK_RETURNS,        TOK_RIGHT,          TOK_RND,        TOK_SEEK,

    TOK_STEP,           TOK_STRING,         TOK_THEN,       TOK_TRUE,
    TOK_TYPE,           TOK_UCASE,          TOK_UNTIL,      TOK_WHILE,

    TOK_WRITE,          TOK_WRITEFILE,

    // A c-specific hack to get the number of tokens,
    // by virtue of it being the last variant in this enum.
    TOK_SIZE,
};

// The purpose of a "null" token, despite its infamous reputation,
// is to be able to assign a quick and dirty sentinel value to something
// WITHOUT worrying about a crash-on-dereference.
//
// Quotes, braces, brackets, parentheses and other strings which are implicitly paired
// are considered as tokens in an attempt to reduce the number of
// responsibilities of the tokenizer; whether they're actually paired or not will be
// determined after tokenization, instead of having the tokenizer exit the program.
const static char *g_token_kinds[] = {
    "NULL_TOKEN",
    "\'",               "\"",               "(",                ")",
    "[",                "]",                "{",                "}",

    "-",                "<-",               "*",                "/",
    "//",               "+",                "<",                "<=",

    "<>",               "=",                ">",                ">=",
    "&",                "AND",              "APPEND",           "ARRAY",

    "BOOLEAN",          "BYREF",            "BYVAL",            "CALL",
    "CASE",             "OF",               "CHAR",             "CLOSEFILE",

    "CONSTANT",         "DATE",             "DECLARE",          "DIV",
    "ELSE",             "ENDCASE",          "ENDFUNCTION",      "ENDIF",

    "ENDPROCEDURE",     "ENDTYPE",          "EOF",              "FALSE",
    "FOR",              "TO",               "FUNCTION",         "GETRECORD",

    "IF",               "INPUT",            "INTEGER",          "LCASE",
    "LENGTH",           "MID",              "MOD",              "NEXT",

    "NOT",              "OPENFILE",         "OR",               "OTHERWISE",
    "OUTPUT",           "PROCEDURE",        "PUTRECORD",        "READ",

    "READFILE",         "REAL",             "REPEAT",           "RETURN",
    "RETURNS",          "RIGHT",            "RND",              "SEEK",

    "STEP",             "STRING",           "THEN",             "TRUE",
    "TYPE",             "UCASE",            "UNTIL",            "WHILE",

    "WRITE",            "WRITEFILE",
};
#define g_token_kinds_sz ((sizeof g_token_kinds) / g_token_kinds[0])

bool test_tokenizer_enum_member_matches_string(void) {
    // printf("LBRACKET is %s\n", g_token_kinds[TOK_LBRACKET]);
    return
        CP_ASSERT(!strcmp(g_token_kinds[TOK_SINGLEQUOTE], "\'")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_DOUBLEQUOTE], "\"")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LPAREN], "(")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RPAREN], ")")) &&

        CP_ASSERT(!strcmp(g_token_kinds[TOK_LBRACE], "{")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RBRACE], "}")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LBRACKET], "[")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RBRACKET], "]")) &&

        CP_ASSERT(!strcmp(g_token_kinds[TOK_MINUS], "-")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LARROW], "<-")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_STAR], "*")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_SLASH], "/")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_2SLASH], "//")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_PLUS], "+")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LT], "<")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LEQ], "<=")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_NEQ], "<>")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_EQ], "=")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_GT], ">")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_GEQ], ">=")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_AMPERSAND], "&")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_AND], "AND")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_APPEND], "APPEND")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ARRAY], "ARRAY")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_BOOLEAN], "BOOLEAN")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_BYREF], "BYREF")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_BYVAL], "BYVAL")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_CALL], "CALL")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_CASE], "CASE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_OF], "OF")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_CHAR], "CHAR")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_CLOSEFILE], "CLOSEFILE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_CONSTANT], "CONSTANT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_DATE], "DATE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_DECLARE], "DECLARE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_DIV], "DIV")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ELSE], "ELSE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ENDCASE], "ENDCASE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ENDFUNCTION], "ENDFUNCTION")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ENDIF], "ENDIF")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ENDPROCEDURE], "ENDPROCEDURE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_ENDTYPE], "ENDTYPE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_EOF], "EOF")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_FALSE], "FALSE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_FOR], "FOR")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_TO], "TO")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_FUNCTION], "FUNCTION")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_GETRECORD], "GETRECORD")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_IF], "IF")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_INPUT], "INPUT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_INTEGER], "INTEGER")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LCASE], "LCASE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_LENGTH], "LENGTH")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_MID], "MID")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_MOD], "MOD")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_NEXT], "NEXT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_NOT], "NOT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_OPENFILE], "OPENFILE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_OR], "OR")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_OTHERWISE], "OTHERWISE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_OUTPUT], "OUTPUT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_PROCEDURE], "PROCEDURE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_PUTRECORD], "PUTRECORD")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_READ], "READ")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_READFILE], "READFILE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_REAL], "REAL")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_REPEAT], "REPEAT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RETURN], "RETURN")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RETURNS], "RETURNS")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RIGHT], "RIGHT")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_RND], "RND")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_SEEK], "SEEK")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_STEP], "STEP")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_STRING], "STRING")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_THEN], "THEN")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_TRUE], "TRUE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_TYPE], "TYPE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_UCASE], "UCASE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_UNTIL], "UNTIL")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_WHILE], "WHILE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_WRITE], "WRITE")) &&
        CP_ASSERT(!strcmp(g_token_kinds[TOK_WRITEFILE], "WRITEFILE")) &&

        1;

}

// In order to have a map of the source code that is trivially traversable, backtracking included,
// We will need to store it as a flat array of tokens.
// The scheme of how such a mapping could look like is demonstrated as follows:
// ```
// 00 // I'm pretty sure this language disallows arrays as a valid type to a procedure parameter
// 00 PROCEDURE Sum3Nums(BYREF Arr: StructWith3NumsT)
// 01   OUTPUT "Numbers to add:", Arr.v0, Arr.v1, Arr.v2
// 02   OUTPUT "Their sum is:" Arr.v0 + Arr.v1 + Arr.v2
// 03 ENDPROCEDURE
// ```
//
// LINE0 PROCEDURE Sum3Nums ( BYREF Arr : StructWithThreeNumsT ) NL
//
// on second thought, backtracking doesnt seem worth it if it's only goal is to be conveniently able to deliver good error messages.
// we're only going to be consuming tokens in the forward direction
//
// to impose the guarantee of our parser beginning at a statement,
// we should define the keywords that are guaranteed to be present at the beginning of a statement and then check
// against them. orienting our problem in terms of handling a sequence of statements will increase implementation speed.
//
// such a procedure could be handled by...
void cp_parse_statement(int *out_tree, char *in_statement);

struct FatToken {
    enum TokenKind kind;
    char *src_mapping_start;
    int token_len_bytes;
};

// Tokenizes a string.
// a better description of its behaviour will be written soon.
void cp_tokenize(
    // Input string, assumed to be ASCII.
    char *str, int str_sz,

    // "token kind" is defined as the offset from g_tokens.
    int *out_tokkind, int tokkind_sz,

    // "token location" refers to the starting character
    // index where a token is found.
    int *out_toklocs, int toklocs_sz
) {
    // Maximal munch.
    // Find longest common subsequence.

    /*
    ENDPROCEDURE
    If c is ":
        lookahead for matching quote. if not found, error.
        tokenize as string literal, with its start and end markers.
    If c is ':
        lookahead for matching quote, if not found, error.
        tokenize as char literal, with start and end markers.
        Note: detailed error handling is to be deferred to the parse stage.
    Does our character have a match? how many matches?
    if no match, tokenize it as an identifier.
    */
}

// -- Main ----
//
// The place where program execution starts.

int main(int arg_count, char **args) {
    CP_ADD_TEST(test_succeed);
    CP_ADD_TEST(test_tokenizer_enum_member_matches_string);
    CP_RUN_TESTS();

    return 0;
}
