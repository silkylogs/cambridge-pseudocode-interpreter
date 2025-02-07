// -- Cambridge pseudocode interpeter ----
//
// The implementation for the Cambridge pseudocode programming langauge.
// Impl notes:
// - The C99 standard guarantees that `int32_t`s are at least 16 bits wide.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

// -- Utility macros ----
//
// A set of macros intended to implement functionality in C a bit less tedious.

#define CP_LEVEL_LOG(LEVEL, PRINTF_FMT_STR, ...) \
    printf("%s:%d:1: " LEVEL ": " PRINTF_FMT_STR, __FILE__, __LINE__, __VA_ARGS__);

#define CP_LOG(FMT_MSG, ...) CP_LEVEL_LOG("Log", FMT_MSG, __VA_ARGS__);
#define CP_WARN(FMT_MSG, ...) CP_LEVEL_LOG("Warning", FMT_MSG, __VA_ARGS__);

static bool cp_assert_internal(bool b, const char *b_expression, const char *file, int32_t line);
static bool cp_assert_internal(bool b, const char *b_expression, const char *file, int32_t line) {
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

// A helpful link for getting to understand C's pointer syntax
// https://cseweb.ucsd.edu/~gbournou/CSE131/rt_lt.rule.html

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

bool test_succeed(void) {
    return 1 + 1 == 2;
}

// -- String ----
//
// An abstraction representing a string of characters.
//
// Contract:
// - The string points to a valid reigon of memory.
// - The size will always be positive.
// - The size will include the null terminator. 
//   Rationale:
//      The decision to have both null termination as well as a size is to
//      be able to interop with c's string functions while retaining the convenience of sized strings.
// - If the size is zero, no null terminator is nessecary.

#ifndef CP_SRC_ASCII_STRING_VIEW
#define CP_SRC_ASCII_STRING_VIEW

typedef struct CP_StringIncludingZeroTerminator {
    char *start;
    int32_t sz;
} CP_StringIncludingZeroTerminator, CP_StrZT;

#define CP_STRING_INCLUDING_ZT_LITERAL(str) (CP_StrZT){ .start=str, .sz=sizeof (str) }
#define SL(str) CP_STRING_INCLUDING_ZT_LITERAL(str)

// -- Substring ----
//
// An abstraction representing a subset of a string.
//
// Contract:
// - Pointer to start of substring will always be at a lower or equal address than the end pointer.
// - The substring points to a valid reigon in memory.
// - The substring will not include any null terminators.

typedef struct CP_InclusiveUnidirectionalSubstring {
    char *start, *end;
} CP_InclusiveUnidirectionalSubstring, CP_SubStr;

#define CP_INCLUSIVE_UNIDIR_SUBSTRING_LITERAL(str) (CP_SubStr){ .start=str, .end=(str + sizeof (str) - 1) }
#define SUBL(str) CP_INCLUSIVE_UNIDIR_SUBSTRING_LITERAL(str)

int32_t CP_SubStr_size(const CP_SubStr *x) {
    // return x->end - x->start + 1;
    // TODO: switch to intptr_t or something instead of sticking to int32
    return 0;
}

// -- from_const_cstr --------------------------

/*
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
}*/

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

typedef struct Iter2 Iter2;
struct Iter2 {
    CP_InclusiveUnidirectionalSubstring substr;
    Iter2 *lt_link, *rt_link;
};

// 1 + 2 + 3
//       ^

// Infinite recursion possible reason - mismatch b/w
// zero terminated c-string and sized strings
//
// I'll stick to the convention (size = (null) terminator - start)
//
// TODO:
// - Manually implement recursion; 
//   we need to be able to specify the recursion depth limit
//   in order to be able to be more deterministic
// - Categorize the data used by the function in order to figure out what to draw.

// TODO: tests.
void cp_find_char_in_unidirectional_substring_linear_reverse(
    char const *str_start,
    char const *str_end,
    char const c,

    char const **out_loc, 
    bool *out_found_explicitly
) {
    char const *op_loc;
    bool op_found;
    {
        char const *const ptr_before_string_start = str_start - 1;
        for (
            op_loc = str_end;
            (*op_loc != c) && (op_loc >= ptr_before_string_start);
            --op_loc
        );
        op_found = (op_loc != ptr_before_string_start) && (*op_loc == c);
    }

    *out_loc = op_loc;
    *out_found_explicitly = op_found;
}

void cp_find_char_reverse(
    CP_InclusiveUnidirectionalSubstring substr,
    char const c,
    char const **out_loc, 
    bool *out_found_explicitly
){
    cp_find_char_in_unidirectional_substring_linear_reverse(
        substr.start, substr.end, c, 
        out_loc, out_found_explicitly);
}

char cp_is_char_num(char const c) { return (c >= '0') && (c <= '9'); }
int32_t cp_char_to_digit(char const c) { return c - '0'; }

/*
    To make a program recursive, but still have control over the stack, we need to keep track of the data.

    Propagation stage, Backpropagation stage.

    [1 + 2 + 3]
    -------
    |     |
    [1 + 2] [3]
    -----   |
    |   |   |
    [1] [2] [3]


    Expected action trace:
    string: 1+2+3
    plus found
    not terminal
    push rt substr to return stack (rs = 3)
    string = lt substr

    string: 1+2
    plus found
    not terminal
    push lt substr to return stack (rs = 3 2)
    string = lt substr

    string: 1
    no plus found
    is terminal
    conv and add to accumulator (1)
    string = pop (rs = 3)

    string: 2
    no plus found
    is terminal
    conv and add to accumulator (2)
    string = pop (rs empty)

    string: 3
    no plus foundd
    is terminal
    conv and add to acc (6)
    rs is empty
    return acc.
*/

int32_t cp_eval_expr2(
    Iter2 str,
    Iter2 *rst // return stack top
) {
    Iter2 const *rst_base = rst;
    int32_t acc = 0;

start:
    // printf("Evaluating: %.*s\n", CP_SubStr_size(&str.substr), str.substr.start);

    char const *split_loc;
    bool found;
    cp_find_char_reverse(
        str.substr, '+',
        &split_loc, &found);
        
    // Is terminal?
    if ((!found) && cp_is_char_num(*str.substr.start)) {
        if (rst < rst_base) { // Stack empty
            // printf("Returning acc (rst < rst_base) = %d\n", acc);
            return acc;
        } else if (rst == rst_base) { // Last elem in stack
            acc += cp_char_to_digit(*str.substr.start);
            // printf("Returning acc (rst == rst_base): %d\n", acc);
            return acc;
        } else { // Multiple elems in stack
            acc += cp_char_to_digit(*str.substr.start);
            // printf("Acc: %d\n", acc);
            str = *rst;
            --rst; // Pop
            goto start;
        }
    } else {
        if (found) {
            // TODO: validate this.
            // TODO: casts are evil
            Iter2 lt, rt;
            lt.substr.start = str.substr.start;
            lt.substr.end = (char*)split_loc - 1;
            rt.substr.start =  (char*)split_loc + 1;
            rt.substr.end = str.substr.end;

            // Push rt
            ++rst;
            *rst = rt;
            
            str = lt;
            goto start;
        }
    }

    // Unreachable
    return 0xCCCCCCCC;
}

bool test_cp_eval_expr2(void) {
    Iter2 it_stk[0x100];
    Iter2 test_iter;
    test_iter.substr = SUBL("1+2+3+4");
    
    return CP_ASSERT(cp_eval_expr2(test_iter, it_stk) == 10);
}

// -- Raylib Backend ----
//
// The raylib interface.

#include "raylib.h"

void cprb_setup(void) {
    InitWindow(800, 800, "Expression evaluation visualizer");
    SetTargetFPS(60);
}

bool cprb_running(void) {
    return !WindowShouldClose();
}

void cprb_draw_centered_box_with_text(
    int32_t const cx, 
    int32_t const cy, 
    int32_t const padding, 
    char const *text
) {
    int32_t const fntsz = 20; 

    int32_t const text_w = MeasureText(text, fntsz);
    int32_t const text_h = fntsz;
    // Assuming (0, 0) is top left corner of screen.
    // (centered_x, centered_y) -> (upper_left_x, upper_left_y)
    int32_t const text_ulx = cx - (text_w / 2);
    int32_t const text_uly = cy - (text_h / 2);

    int32_t const box_w = text_w + (padding * 2);
    int32_t const box_h = text_h;
    int32_t const box_ulx = text_ulx - padding;
    int32_t const box_uly = text_uly;

    DrawRectangleLines(box_ulx, box_uly, box_w, box_h, BLACK);
    DrawText(text, text_ulx, text_uly, fntsz, BLACK);
};

typedef struct CP_CenteredBoxedText CP_BoxedText;
struct CP_CenteredBoxedText {
    int32_t cx, cy;
    int32_t padding;
    char const *text;
};

void (box)(CP_BoxedText b) {
    cprb_draw_centered_box_with_text(b.cx, b.cy, b.padding, b.text);
}

void cprb_draw_centered_line_between_boxes(CP_BoxedText src, CP_BoxedText dst) {
    DrawLine(src.cx, src.cy, dst.cx, dst.cy, LIGHTGRAY);
}

typedef struct CP_BoxedTextBTreeNode CP_TextNode;
struct CP_BoxedTextBTreeNode {
    CP_BoxedText btxt;
    CP_TextNode *lt, *rt;
};

// A test
CP_TextNode *generate_tree(void) {
    static CP_TextNode tree_data[5] = {0};
    static char n0[] = "+";
    static char n1[] = "3";
    static char n2[] = "+";
    static char n3[] = "2";
    static char n4[] = "1";
    int32_t const padding = 2;

    // Modelling 1+2+3 (+ 3 (+ 2 1)) (op l r)
    CP_TextNode *ptr;
    
    ptr = tree_data + 0;
    ptr->btxt.text = n0;
    ptr->btxt.cy = 100;
    ptr->btxt.cx = 100;
    ptr->btxt.padding = padding;
    ptr->lt = tree_data + 1;
    ptr->rt = tree_data + 2;
    
    ptr = tree_data + 1;
    ptr->btxt.text = n1;
    ptr->btxt.cy = 200;
    ptr->btxt.cx = 100;
    ptr->btxt.padding = padding;
    ptr->lt = NULL;
    ptr->rt = NULL;
    
    ptr = tree_data + 2;
    ptr->btxt.text = n2;
    ptr->btxt.cy = 200;
    ptr->btxt.cx = 200;
    ptr->btxt.padding = padding;
    ptr->lt = tree_data + 3;
    ptr->rt = tree_data + 4;

    ptr = tree_data + 3;
    ptr->btxt.text = n3;
    ptr->btxt.cy = 300;
    ptr->btxt.cx = 200;
    ptr->btxt.padding = padding;
    ptr->lt = NULL;
    ptr->rt = NULL;

    ptr = tree_data + 4;
    ptr->btxt.text = n4;
    ptr->btxt.cy = 300;
    ptr->btxt.cx = 300;
    ptr->btxt.padding = padding;
    ptr->lt = NULL;
    ptr->rt = NULL;

    return tree_data;
}

void draw_tree_iter(CP_TextNode *node) {
    if (node) {
        box(node->btxt);
        if (node->lt) cprb_draw_centered_line_between_boxes(node->btxt, node->lt->btxt);
        if (node->rt) cprb_draw_centered_line_between_boxes(node->btxt, node->rt->btxt);
    }
}

void draw_tree(CP_TextNode *root) {
    if (!root) return;
    draw_tree_iter(root);
    draw_tree(root->lt);
    draw_tree(root->rt);
}

bool is_leaf(CP_TextNode *tree_root) {
    return tree_root->lt == NULL && tree_root->rt == NULL;
}

bool is_penultimate(CP_TextNode *root) {
    if (!root) return false;
    return is_leaf(root)?
        false :
        is_leaf(root->lt) && is_leaf(root->rt);
}

CP_TextNode *find_penultimate(CP_TextNode *root) {
    if (!root) return NULL;
    if (is_penultimate(root)) return root;
    
    CP_TextNode* lt = find_penultimate(root->lt);
    CP_TextNode* rt = find_penultimate(root->rt);

    if (rt) return rt;
    else if (lt) return lt;
    else return NULL;
}

// -- Forth ----
//
// The internal forth implementation, intended to make development easier.
//

typedef uintptr_t Cell;

// "Quits" to the top level interpreter, clearing the stack
void quit(void);

// Stack
//
// Usage:

// Returns the top of the data stack, or quits to interpreter immediately
Cell top(void);


// Usage:
// To compile `: double dup + ;`, then execute `5 double .`,
// ```
// colon("double");
// doword("dup");
// doword("+");
// semicolon();
// ```
//

// Start compiling the definition of a word
void colon(char const *const word);

// Finish compiling a word
void semicolon(void);


// Top level interpreter
//
// Pass a word to the interpreter to either compile or execute it.
//
// Usage:
// ```
// doword("5");
// doword("5");
// doword("+");
// doword("."); // Prints 10
// ```


#define CTX_MEM_SZ 0x100
#define CTX_STRBUF_SZ 0x100
#define CTX_PRIM_SZ 0x100
typedef struct Context Ctx;
typedef void (*PrimitiveFunction)(Ctx *);
struct Context {
    Cell
        mem[CTX_MEM_SZ],
        latest, head,
        top, rtop,
        ip,
        dummy;
    
    char
        strbuf[CTX_STRBUF_SZ], // Containing buffer
        *(strs[CTX_STRBUF_SZ]); // Ptr to zero terminated strings
    Cell strtop; // Topmost index to ptr to ztstrings

    enum State {
        STATE_INTERPRETING,
        STATE_COMPILING,
        STATE_QUIT,
    } state;

    PrimitiveFunction primitives[CTX_PRIM_SZ];
    Cell primtop;
};
Ctx ctx = {0};

enum DictEntry {
    ENTRY_OFFSET_NEXT,
    ENTRY_OFFSET_FHIDDEN,
    ENTRY_OFFSET_FIMMED,
    ENTRY_OFFSET_NAME,
    ENTRY_OFFSET_CODEWORD,
    ENTRY_SIZE,
};
Cell entry_next(Cell const addr) { return addr + ENTRY_OFFSET_NEXT; }
Cell entry_fhidden(Cell const addr) { return addr + ENTRY_OFFSET_FHIDDEN; }
Cell entry_fimmed(Cell const addr) { return addr + ENTRY_OFFSET_FIMMED; }
Cell entry_name(Cell const addr) { return addr + ENTRY_OFFSET_NAME; }
Cell entry_codeword(Cell const addr) { return addr + ENTRY_OFFSET_CODEWORD; }

void stringcontainerinit(void) {
    ctx.strbuf[0] = 0;
    ctx.strtop = 1;
    for (Cell i = 0; i < CTX_STRBUF_SZ; ctx.strs[i++] = ctx.strbuf); 
}

void comma(Cell const);
void writeentry_bumphead(
    Cell const next, 
    Cell const ishidden, 
    Cell const isimmediate, 
    Cell const name,
    Cell const codeptr
) {
    comma(next);
    comma(ishidden);
    comma(isimmediate);
    comma(name);
    comma(codeptr);
}

void stringcontainerinit(void);
void writeentry_bumphead(
    Cell const next, 
    Cell const ishidden, 
    Cell const isimmediate, 
    Cell const name,
    Cell const codeptr
);
void dictinit(void) {
    stringcontainerinit();

    // init dict vars
    ctx.latest = 0 + ENTRY_SIZE;
    ctx.head = 0 + ENTRY_SIZE;
    
    Cell const 
        ptr_to_nothing = 0,
        is_very_hidden = 1,
        not_immediate = 0,
        no_name = 0;
    writeentry_bumphead(ptr_to_nothing, is_very_hidden, not_immediate, no_name, ptr_to_nothing);
}

void writeentry_bumphead(
    Cell const next, 
    Cell const ishidden, 
    Cell const isimmediate, 
    Cell const name,
    Cell const codeptr
);
void addentry_withcodeptr(
    Cell const ishidden, 
    Cell const isimmed, 
    Cell const name,
    Cell const codeptr
) {
    Cell newlatest = ctx.head;
    Cell lastentry = ctx.latest;
    writeentry_bumphead(lastentry, ishidden, isimmed, name, codeptr);
}

void addentry_withcodeptr(
    Cell const ishidden, 
    Cell const isimmed, 
    Cell const name,
    Cell const codeptr
);
void addentry(
    Cell const ishidden, 
    Cell const isimmed,
    Cell const name
) {
    Cell const dummy = 0;
    addentry_withcodeptr(ishidden, isimmed, name, dummy);
    ctx.mem[ctx.latest - 1] = ctx.latest; // Fixup
}

Cell latest(void) {
    return ctx.latest;
}

// TODO: checked memory access, that throws an exception/quits
Cell latest(void);
Cell entry_name(Cell const addr);
Cell entry_next(Cell const addr);
Cell dictcontains(char const *const word) {
    Cell adr_entry = latest();
    do {
        Cell name_idx = entry_name(adr_entry);
        char const *const lookup = ctx.strs[name_idx];
        int strcmp_result = strcmp(word, lookup);
        if (0 == strcmp_result) return adr_entry;
        adr_entry = ctx.mem[entry_next(adr_entry)];
    } while (adr_entry);
    return adr_entry;
}

bool isnum(char const *const word) {
    // Return value:
    // Number of receiving arguments successfully assigned 
    // (which may be zero in case a matching failure occurred 
    // before the first receiving argument was assigned), 
    // or EOF if input failure occurs before the first receiving argument was assigned.
    int32_t dummy;
    int32_t rv = sscanf(word, "%d", &dummy);
    if (rv == 1) return true;
    if (rv == 0 || rv == EOF) return false;
    return false;
}

void push(Cell const num) {
    ctx.top++;
    ctx.mem[ctx.top] = num;
}

void push(Cell const num);
void lit(Cell const num) {
    push(num);
}

Cell conv2num(char const *const word) {
    return atoll(word);
}

bool isprimitive(Cell const addr) { 
    return 0; // TODO
}

void next(void) {
    ctx.ip = ctx.mem[ctx.rtop];
    ctx.rtop--;
}

void next(void);
void execprimitive(Cell const prim_idx) {
    (ctx.primitives[prim_idx])(&ctx);
    next();
}

void rpush(Cell const num) {
    ctx.mem[ctx.rtop + 1] = num;
    ctx.rtop++;
}

void rpush(Cell const num);
void execnonprimitive(Cell const addr) {
    rpush(ctx.ip + 1);
    ctx.ip = ctx.mem[ctx.ip];
}

void allot(Cell const cells) {
    ctx.head += cells;
}

void allot(Cell const);
void comma(Cell const addr) {
    ctx.mem[ctx.head] = addr;
    allot(1);
}

bool isprimitive(Cell const addr);
void execprimitive(Cell const addr);
void execnonprimitive(Cell const addr);
void comma(Cell const addr);
void exec(Cell const addr) {
    if (ctx.state == STATE_INTERPRETING) {
        if (isprimitive(addr)) execprimitive(addr);
        else execnonprimitive(addr);
    } else if (ctx.state == STATE_COMPILING) {
        comma(addr);
    }
}

void drop(void) {
    ctx.top = 0;
}

void rdrop(void) {
    ctx.rtop = 0;
}

void drop(void);
void rdrop(void);
void quit(void) {
    drop();
    rdrop();
    ctx.state = STATE_QUIT;
}


void quit(void);
Cell dictcontains(char const *const word);
void exec(Cell const);
bool isnum(char const *const word);
void lit(Cell const);
Cell conv2num(char const *const word);
void error(char const *const str);
void quit(void);
void doword(char const *const word) {
    Cell addr = dictcontains(word);
    if (addr) {
        exec(addr);
    } else {
        if (isnum(word)) {
            lit(conv2num(word));
        } else {
            printf("Word %s not found", word);
            quit();
        }
    }
}

void init(void) {
    dictinit();
}

Cell top(void) {
    return ctx.mem[ctx.top];
}



bool test_forth(void) {
    init();
    doword("5");
    return top() == 5;
}

// -- Main ----
//
// The place where program execution starts.

int32_t main(int32_t arg_count, char const *const *args) {
    CP_ADD_TEST(test_succeed);
    CP_ADD_TEST(test_tokenizer_enum_member_matches_string);
    CP_ADD_TEST(test_cp_eval_expr2);
    CP_ADD_TEST(test_forth);
    CP_RUN_TESTS();

    /*
    cprb_setup();

    // ToggleBorderlessWindowed();

    CP_TextNode *tree_root = generate_tree();
    // add_tree(tree_root);

    while (cprb_running())
    {
        double const pausedur = 2.0;
        enum State { PAUSED, ANIMATING };
        static int32_t s = PAUSED;
        static CP_TextNode *pn;

        printf("State: %s\n", s==PAUSED? "PAUSED" : "ANIMATING");
        switch (s) {
            case PAUSED: {
                WaitTime(pausedur);
                s = ANIMATING;
            } break;
            case ANIMATING: {
                // collapse_leaf(tree);
                pn = find_penultimate(tree_root);

                pn->lt->btxt.cx = pn->btxt.cx + (((pn->lt->btxt.cx - pn->btxt.cx) * (0.99 * 1000)) / 1000);
                pn->lt->btxt.cy = pn->btxt.cy + (((pn->lt->btxt.cy - pn->btxt.cy) * (0.99 * 1000)) / 1000);
                pn->rt->btxt.cx = pn->btxt.cx + (((pn->rt->btxt.cx - pn->btxt.cx) * (0.99 * 1000)) / 1000);
                pn->rt->btxt.cy = pn->btxt.cy + (((pn->rt->btxt.cy - pn->btxt.cy) * (0.99 * 1000)) / 1000);

                if (   pn->lt->btxt.cx == pn->btxt.cx 
                    && pn->lt->btxt.cy == pn->btxt.cy
                    && pn->rt->btxt.cx == pn->btxt.cx 
                    && pn->rt->btxt.cy == pn->btxt.cy)
                {
                    int32_t lt = *pn->lt->btxt.text - '0';
                    int32_t rt = *pn->rt->btxt.text - '0';
                    *(char*)pn->btxt.text = '0' + lt + rt;
                    pn->rt = NULL;
                    pn->lt = NULL;
                    s = PAUSED;
                }
            } break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_tree(tree_root);
        EndDrawing();
    }

    CloseWindow();

    */
    return 0;
}
