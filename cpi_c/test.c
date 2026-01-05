#include <assert.h>
#include "vm.h"

// -----------------------------------------------------------------


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
        printf(
            "Test %5.5d of %5.5d: \"%s\"... %s\n",
            i + 1, ts.ptr_to_top + 1,
            ts.names[i],
            (ok = (ts.fns[i])()) ? "Ok" : "Error"
        );
        all_ok &= ok;
    }

    CP_LEVEL_LOG(
        "Summary", "%s\n",
        all_ok ? "All tests passed" : "One or more tests failed"
    );
}

// ---------------------------------------------------------
// Note for testing:
// - Test preconditions
// - Test postconditions

bool test__vm_decl_var__atomic_dt__INTEGER(void) {
    bool test_ok = false;
    struct VmState state = vm_init();
    struct Instr i = instr_decl_var("Foo", "INTEGER");;

    vm_exe_instr(&state, &i);
    instr_free(i);

    struct Var *var = vm_search_var_by_name(state, "Foo");
    if (var) {
        test_ok = 0 == strcmp(var->name, "Foo")
            && 0 == strcmp(var->value, "0")
            && 0 == strcmp(var->typename, "INTEGER");
    } else {
        printf("Expected search for \"%s\" to succeed.\n", "Foo");
        test_ok = false;
    }

    vm_free(&state);
    return test_ok;
}

bool test__vm_decl_var__atomic_dt__REAL(void) {
    struct VmState state = vm_init();
    struct Instr i;
    bool test_ok = false;

    i = instr_decl_var("Foo", "REAL");
    vm_exe_instr(&state, &i);
    instr_free(i);

    struct Var *var = vm_search_var_by_name(state, "Foo");
    if (var) {
        test_ok = 0 == strcmp(var->name, "Foo")
            && 0 == strcmp(var->value, "0.0")
            && 0 == strcmp(var->typename, "REAL");
    } else {
        printf("Expected search for \"%s\" to succeed.\n", "Foo");
        test_ok = false;
    }

    vm_free(&state);
    return test_ok;
}

bool test__vm_decl_var__atomic_dt__CHAR(void) {
    struct VmState state = vm_init();
    struct Instr i;
    bool test_ok = false;

    i = instr_decl_var("Foo", "CHAR");
    vm_exe_instr(&state, &i);
    instr_free(i);

    struct Var *var = vm_search_var_by_name(state, "Foo");
    if (var) {
        test_ok = 0 == strcmp(var->name, "Foo")
            && 0 == strcmp(var->value, "\0")
            && 0 == strcmp(var->typename, "CHAR");
    } else {
        printf("Expected search for \"%s\" to succeed.\n", "Foo");
        test_ok = false;
    }

    vm_free(&state);
    return test_ok;
}

bool test__vm_decl_var__atomic_dt__STRING(void) {
    struct VmState state = vm_init();
    struct Instr i;
    bool test_ok = false;

    i = instr_decl_var("Foo", "STRING");
    vm_exe_instr(&state, &i);
    instr_free(i);

    struct Var *var = vm_search_var_by_name(state, "Foo");
    if (var) {
        test_ok = 0 == strcmp(var->name, "Foo")
            && 0 == strcmp(var->value, "\0")
            && 0 == strcmp(var->typename, "STRING");
    } else {
        printf("Expected search for \"%s\" to succeed.\n", "Foo");
        test_ok = false;
    }

    vm_free(&state);
    return test_ok;
}


bool test__vm_decl_var__atomic_dt__DATE(void) {
    struct VmState state = vm_init();
    struct Instr i;
    bool test_ok = false;

    i = instr_decl_var("Foo", "DATE");
    vm_exe_instr(&state, &i);
    instr_free(i);

    struct Var *var = vm_search_var_by_name(state, "Foo");
    if (var) {
        test_ok = 0 == strcmp(var->name, "Foo")
            && 0 == strcmp(var->value, "00/00/0000")
            && 0 == strcmp(var->typename, "DATE");
    } else {
        printf("Expected search for \"%s\" to succeed.\n", "Foo");
        test_ok = false;
    }

    vm_free(&state);
    return test_ok;
}

bool test__vm_decl_var__atomic_dt__BOOLEAN(void) {
    struct VmState state = vm_init();
    struct Instr i;
    bool test_ok = false;

    i = instr_decl_var("Foo", "BOOLEAN");
    vm_exe_instr(&state, &i);
    instr_free(i);

    struct Var *var = vm_search_var_by_name(state, "Foo");
    if (var) {
        test_ok = 0 == strcmp(var->name, "Foo")
            && 0 == strcmp(var->value, "FALSE")
            && 0 == strcmp(var->typename, "BOOLEAN");
    } else {
        printf("Expected search for \"%s\" to succeed.\n", "Foo");
        test_ok = false;
    }

    vm_free(&state);
    return test_ok;
}

// ---------------------------------------------------------

int main(void) {
    CP_ADD_TEST(test__vm_decl_var__atomic_dt__INTEGER);
    CP_ADD_TEST(test__vm_decl_var__atomic_dt__REAL);
    CP_ADD_TEST(test__vm_decl_var__atomic_dt__CHAR);
    CP_ADD_TEST(test__vm_decl_var__atomic_dt__DATE);
    CP_ADD_TEST(test__vm_decl_var__atomic_dt__STRING);
    CP_ADD_TEST(test__vm_decl_var__atomic_dt__BOOLEAN);

    CP_RUN_TESTS();

    return 0;
}
