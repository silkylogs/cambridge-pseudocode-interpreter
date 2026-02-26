#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include "lblcont.h"
#include "vm.h"

// ---------------------------------------------------------

#define TEST(str) \
    this_test_name = str; \
    ok = true; 

#define TESTEND \
    printf("%3.3d: %12.12s test \"%s\"\n", test_no, (ok ? "Succeeding" : "Failing"), this_test_name); \
    all_test_ok &= ok; \
    test_no += 1;

#define EXPECT(expr) ok &= (expr);

int main(void) {
    char *this_test_name = NULL;
    bool ok = 0; // Did this test pass?
    bool all_test_ok = true;
    int test_no = 1;

    {
        TEST("Declare");
        EXPECT(upper('d') == 'D');
        EXPECT(upper('e') == 'E');
        EXPECT(upper('c') == 'C');
        EXPECT(upper('l') == 'L');
        EXPECT(upper('a') == 'A');
        EXPECT(upper('r') == 'R');
        EXPECT(upper('e') == 'E');
        TESTEND;
    }

    {
        TEST("vm_guess_stmt_kind_from_first_word DECLARE");
        size_t len;
        enum StatementGuess g0, g1, g2;

        vm_guess_stmt_kind_from_first_word("Declare", &g0, &len);
        vm_guess_stmt_kind_from_first_word("declare", &g1, &len);
        vm_guess_stmt_kind_from_first_word("DECLARE", &g2, &len);

        EXPECT(g0 == STMT_DECLARE);
        EXPECT(g1 == STMT_DECLARE);
        EXPECT(g2 == STMT_DECLARE);
        EXPECT(len == sizeof "DECLARE" - 1);
        TESTEND;
    }

    {
        TEST("vm_exec_stmt DECLARE name and type extraction");
        struct VmState state = { 0 };

        char stmt0[] = "DECLARE Foo : Bar";
        char stmt1[] = "DECLARE LongerVarName : LongerTypeName";

        vm_exec_stmt(&state, &stmt0);
        vm_exec_stmt(&state, &stmt1);

        EXPECT(0 == strcmp(state.vars[0].name, "Foo"));
        EXPECT(0 == strcmp(state.vars[0].type, "Bar"));
        EXPECT(0 == strcmp(state.vars[1].name, "LongerV"));
        EXPECT(0 == strcmp(state.vars[1].type, "LongerT"));
        TESTEND;
    }

    {
        TEST("progmem");

        size_t exp_sz = 0;
        size_t exp_cap = 50;

        struct ProgMem pm = pmnew(50);
        EXPECT(exp_sz == pm.sz && exp_cap == pm.cap);
        pmfree(&pm);

        TESTEND;
    }

    {
        TEST("container");

        cell expected_content = 0xDEADBEEF;

        struct ProgMem pm = pmnew(100);

        lbl_insert(&pm, "A bunch of nothing", 0);
        lbl_insert(&pm, "label0", expected_content);

        cell actual_content = lbl_find(&pm, "label0");
        cell actual_nothing = lbl_find(&pm, "Not found");

        pmfree(&pm);

        EXPECT(expected_content == actual_content && !actual_nothing);

        TESTEND;
    }

    {
        TEST("vm_exec_stmt DECLARE atomicdt default value");

        struct VmState state = { 0 };

        char *stmts[] = {
            "DECLARE VarI : Integer",
            "DECLARE VarR : Real",
            "DECLARE VarC : Char",
            "DECLARE VarS : String",
            "DECLARE VarB : Boolean",
            "DECLARE VarD : Date",
        };

        for (size_t i = 0; i < sizeof stmts / sizeof stmts[0]; i += 1) {
            vm_exec_stmt(&state, stmts[i]);
        }

        EXPECT(0 == strcmp(state.vars[0].name, "VarI"));
        EXPECT(0 == strcmp(state.vars[0].type, "Integer"));
        EXPECT(state.vars[0].valcnt == 1);
        EXPECT(state.vars[0].val_arr_starting_idx == 0);
        EXPECT(state.vars[0].valesz == sizeof(int));
        EXPECT(*(int *)state.vars[0].valdat == 0);

        EXPECT(0 == strcmp(state.vars[1].name, "VarR"));
        EXPECT(0 == strcmp(state.vars[1].type, "Real"));
        EXPECT(state.vars[1].valcnt == 1);
        EXPECT(state.vars[1].val_arr_starting_idx == 0);
        EXPECT(state.vars[1].valesz == sizeof(double));
        EXPECT(*(double *)state.vars[1].valdat == 0.0);

        EXPECT(0 == strcmp(state.vars[2].name, "VarC"));
        EXPECT(0 == strcmp(state.vars[2].type, "Char"));
        EXPECT(state.vars[2].valcnt == 1);
        EXPECT(state.vars[2].val_arr_starting_idx == 0);
        EXPECT(state.vars[2].valesz == sizeof(char));
        EXPECT(*(char *)state.vars[2].valdat == '\0');

        EXPECT(0 == strcmp(state.vars[3].name, "VarS"));
        EXPECT(0 == strcmp(state.vars[3].type, "String"));
        EXPECT(state.vars[3].valcnt == 1);
        EXPECT(state.vars[3].val_arr_starting_idx == 0);
        EXPECT(state.vars[3].valesz == sizeof "");
        EXPECT(0 == strcmp((char *)state.vars[3].valdat, ""));

        EXPECT(0 == strcmp(state.vars[4].name, "VarB"));
        EXPECT(0 == strcmp(state.vars[4].type, "Boolean"));
        EXPECT(state.vars[4].valcnt == 1);
        EXPECT(state.vars[4].val_arr_starting_idx == 0);
        EXPECT(state.vars[4].valesz == sizeof(bool));
        EXPECT(*(bool *)state.vars[4].valdat == false);

        EXPECT(0 == strcmp(state.vars[5].name, "VarD"));
        EXPECT(0 == strcmp(state.vars[5].type, "Date"));
        EXPECT(state.vars[5].valcnt == 1);
        EXPECT(state.vars[5].val_arr_starting_idx == 0);
        EXPECT(state.vars[5].valesz == sizeof "00/00/0000");
        EXPECT(0 == strcmp((char *)state.vars[5].valdat, "00/00/0000"));

        TESTEND;
    }

    //TEST("test__example__comments");
    //{
    //} TESTEND;

    //TEST("test__example__variable_declarations");
    //{
    //} TESTEND;

    //TEST("test__example__CONSTANT_declarations");
    //{
    //} TESTEND;

    //TEST("test__example__assignments");
    //{
    //} TESTEND;

    //TEST("test__example__array_declaration");
    //{
    //} TESTEND;

    //TEST("test__example__using_arrays");
    //{
    //} TESTEND;

    //TEST("test__example__assigning_an_array");
    //{
    //} TESTEND;

    //TEST("test__example__A_statement_should_not_however_refer_to_a_group_of_array_elements_individually");
    //{
    //} TESTEND;

    //TEST("test__example__assigning_a_group_of_array_elements");
    //{
    //} TESTEND;

    //TEST("test__example__declaration_of_custom_type");
    //{
    //} TESTEND;

    //TEST("test__example__using_custom_types");
    //{
    //} TESTEND;

    //TEST("test__example__INPUT_and_OUTPUT_statements");
    //{
    //} TESTEND;

    //TEST("test__example__nested_IF_statements");
    //{
    //} TESTEND;

    //TEST("test__example__formatted_CASE_statement");
    //{
    //} TESTEND;

    //TEST("test__example__nested_FOR_loops");
    //{
    //} TESTEND;

    //TEST("test__example__REPEAT_UNTIL_statement");
    //{
    //} TESTEND;

    //TEST("test__example__WHILE_loop");
    //{
    //} TESTEND;

    //TEST("test__example__use_of_procedures_with_and_without_parameters");
    //{
    //} TESTEND;

    //TEST("test__example__definition_and_use_of_a_function");
    //{
    //} TESTEND;

    //TEST("test__example__passing_parameters_by_reference");
    //{
    //} TESTEND;

    //TEST("test__example__file_handling_operations");
    //{
    //} TESTEND;

    //TEST("test__example__handling_random_files");
    //{
    //} TESTEND;


    printf("\nSummary: %s\n", all_test_ok ? "All tests passed" : "One or more tests failed");
    return 0;
}


