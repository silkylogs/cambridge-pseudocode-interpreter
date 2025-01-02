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
Tests g_tests = {
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