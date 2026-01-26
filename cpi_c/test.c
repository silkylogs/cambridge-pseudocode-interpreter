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

static bool test__upper(void) {
    bool ok = true;
    ok &= upper('d') == 'D';
    ok &= upper('e') == 'E';
    ok &= upper('c') == 'C';
    ok &= upper('l') == 'L';
    ok &= upper('a') == 'A';
    ok &= upper('r') == 'R';
    ok &= upper('e') == 'E';
    return ok;
}

static bool test__vm_guess_stmt_kind_from_first_word__DECLARE(void) {
    bool ok = true;

    size_t len;
    enum StatementGuess g0, g1, g2;

    vm_guess_stmt_kind_from_first_word("Declare", &g0, &len);
    vm_guess_stmt_kind_from_first_word("declare", &g1, &len);
    vm_guess_stmt_kind_from_first_word("DECLARE", &g2, &len);

    ok &= g0 == STMT_DECLARE;
    ok &= g1 == STMT_DECLARE;
    ok &= g2 == STMT_DECLARE;
    ok &= len == sizeof "DECLARE" - 1;

    return ok;
}

static bool test__vm_exec_stmt__DECLARE(void) {
    bool ok = true;
    struct VmState state = { 0 };

    char stmt0[] = "DECLARE Foo : Bar";
    char stmt1[] = "DECLARE LongerVarName : LongerTypeName";

    vm_exec_stmt(&state, &stmt0);
    vm_exec_stmt(&state, &stmt1);

    // TODO: state.vars == Foo && state.types == bar
    ok &= 0 == strcmp(state.vars[0].name, "Foo");
    ok &= 0 == strcmp(state.vars[0].type, "Bar");

    ok &= 0 == strcmp(state.vars[1].name, "LongerV");
    ok &= 0 == strcmp(state.vars[1].type, "LongerT");

    return ok;
}

static bool test__example__comments(void) {
    char program[] =
        "// This procedure swaps\n"
        "// values of X and Y\n"
        "PROCEDURE SWAP(BYREF X : INTEGER, Y INTEGER)\n"
        "    Temp ← X // temporarily store X\n"
        "    X ← Y\n"
        "    Y ← Temp\n"
        "ENDPROCEDURE"; // purposefully not terminate with a newline

    return false;
}

static bool test__example__variable_declarations(void) {
    char program[] =
        "DECLARE Counter : INTEGER\n"
        "DECLARE TotalToPay : REAL\n"
        "DECLARE GameOver : BOOLEAN\n";
    return false;
}

static bool test__example__CONSTANT_declarations(void) {
    char program[] =
        "CONSTANT HourlyRate = 6.50\n"
        "CONSTANT DefaultText = \"N/A\"\n";
    return false;
}

static bool test__example__assignments(void) {
    char program[] =
        "Counter <- 0\n"
        "Counter <- Counter + 1\n"
        "TotalToPay <- NumberOfHours * HourlyRate\n";

    return false;
}

static bool test__example__array_declaration(void) {
    char program[] =
        "DECLARE StudentNames : ARRAY[1:30] OF STRING\n"
        "DECLARE NoughtsAndCrosses : ARRAY[1:3,1:3] OF CHAR\n";
    return false;
}

static bool test__example__using_arrays(void) {
    // Purposeful inclusion of the "" sign here. 
    // This character may not render correctly in your text editor.
    char program[] =
        "StudentNames[1]  \"Ali\"\n"
        "NoughtsAndCrosses[2,3]  ꞌXꞌ\n"
        "StudentNames[n+1]  StudentNames[n] \n";
    return false;
}

static bool test__example__assigning_an_array(void) {
    char program[] = "SavedGame  NoughtsAndCrosses";
    return false;
}

static bool test__example__A_statement_should_not_however_refer_to_a_group_of_array_elements_individually(void) {
    char program[] = "StudentNames [1 TO 30]  \"\"\n";
    return false;
}

static bool test__example__assigning_a_group_of_array_elements(void) {
    char program[] =
        "FOR Index = 1 TO 30\n"
        "   StudentNames[Index] ← \"\"\n"
        "ENDFOR Index\n";
    return false;
}

static bool test__example__declaration_of_custom_type(void) {
    char program[] =
        "TYPE Student\n"
        "   DECLARE Surname : STRING\n"
        "   DECLARE FirstName : STRING\n"
        "   DECLARE DateOfBirth : DATE\n"
        "   DECLARE YearGroup : INTEGER\n"
        "   DECLARE FormGroup : CHAR\n"
        "ENDTYPE\n";
    return false;
}

static bool test__example__using_custom_types(void) {
    char program[] =
        "TYPE Student\n"
        "   DECLARE Surname : STRING\n"
        "   DECLARE FirstName : STRING\n"
        "   DECLARE DateOfBirth : DATE\n"
        "   DECLARE YearGroup : INTEGER\n"
        "   DECLARE FormGroup : CHAR\n"
        "ENDTYPE\n"
        "\n"
        "DECLARE Pupil1 : Student\n"
        "DECLARE Pupil2 : Student\n"
        "DECLARE Form : ARRAY[1:30] OF Student\n"
        "\n"
        "Pupil1.Surname ← \"Johnson\"\n"
        "Pupil1.Firstname ← \"Leroy\"\n"
        "Pupil1.DateOfBirth ← 02/01/2005\n"
        "Pupil1.YearGroup ← 6\n"
        "Pupil1.FormGroup ← ꞌAꞌ\n"
        "Pupil2 ← Pupil1\n"
        "\n"
        "FOR Index ← 1 TO 30\n"
        "   Form[Index].YearGroup ← Form[Index].YearGroup + 1\n"
        "ENDFOR Index\n";
    return false;
}

static bool test__example__INPUT_and_OUTPUT_statements(void) {
    char program[] =
        "INPUT Answer\n"
        "OUTPUT Score\n"
        "OUTPUT \"You have \", Lives, \" lives left\"\n";
    return false;
}

static bool test__example__nested_IF_statements(void) {
    char program[] =
        "IF ChallengerScore > ChampionScore\n"
        "   THEN\n"
        "       IF ChallengerScore > HighestScore\n"
        "           THEN\n"
        "               OUTPUT ChallengerName, \" is champion and highest scorer\"\n"
        "           ELSE\n"
        "               OUTPUT Player1Name, \" is the new champion\"\n"
        "       ENDIF\n"
        "   ELSE\n"
        "       OUTPUT ChampionName, \" is still the champion\"\n"
        "       IF ChampionScore > HighestScore\n"
        "           THEN\n"
        "               OUTPUT ChampionName, \" is also the highest scorer\"\n"
        "       ENDIF\n"
        "ENDIF\n";
    return false;
}

static bool test__example__formatted_CASE_statement(void) {
    char program[] =
        "INPUT Move\n"
        "CASE OF Move\n"
        "   ꞌWꞌ: Position  Position – 10\n"
        "   ꞌSꞌ: Position  Position + 10\n"
        "   ꞌAꞌ: Position  Position – 1\n"
        "   ꞌDꞌ: Position  Position + 1\n"
        "   OTHERWISE : Beep\n"
        "ENDCASE\n";
    return false;
}

static bool test__example__nested_FOR_loops(void) {
    char program[] =
        "Total = 0\n"
        "   FOR Row = 1 TO MaxRow\n"
        "   RowTotal = 0\n"
        "   FOR Column = 1 TO 10\n"
        "       RowTotal  RowTotal + Amount[Row, Column]\n"
        "   ENDFOR Column\n"
        "   OUTPUT \"Total for Row \", Row, \" is \", RowTotal\n"
        "   Total  Total + RowTotal\n"
        "ENDFOR Row\n"
        "OUTPUT \"The grand total is \", Total\n";
    return false;
}

static bool test__example__REPEAT_UNTIL_statement(void) {
    char program[] =
        "REPEAT\n"
        "   OUTPUT \"Please enter the password\"\n"
        "   INPUT Password\n"
        "UNTIL Password = \"Secret\"\n";
    return false;
}

static bool test__example__WHILE_loop(void) {
    char program[] =
        "WHILE Number > 9 DO\n"
        "   Number  Number – 9\n"
        "ENDWHILE\n";
    return false;
}

static bool test__example__use_of_procedures_with_and_without_parameters(void) {
    char program[] =
        "PROCEDURE DefaultSquare\n"
        "   CALL Square(100)\n"
        "ENDPROCEDURE\n"
        "\n"
        "PROCEDURE Square(Size : integer)\n"
        "   FOR Side = 1 TO 4\n"
        "       MoveForward Size\n"
        "       Turn 90\n"
        "   ENDFOR\n"
        "ENDPROCEDURE\n"
        "\n"
        "IF Size = Default\n"
        "   THEN\n"
        "       CALL DefaultSquare\n"
        "   ELSE\n"
        "   CALL Square(Size)\n"
        "ENDIF\n";
    return false;
}

static bool test__example__definition_and_use_of_a_function(void) {
    char program[] =
        "FUNCTION Max(Number1:INTEGER, Number2:INTEGER) RETURNS INTEGER\n"
        "   IF Number1 > Number2\n"
        "       THEN\n"
        "           RETURN Number1\n"
        "       ELSE\n"
        "           RETURN Number2\n"
        "   ENDIF\n"
        "ENDFUNCTION\n"
        "\n"
        "OUTPUT \"Penalty Fine = \", Max(10,Distance*2)\n";
    return false;
}

static bool test__example__passing_parameters_by_reference(void) {
    char program[] =
        "PROCEDURE SWAP(BYREF X : INTEGER, Y : INTEGER)\n"
        "   Temp  X\n"
        "   X  Y\n"
        "   Y  Temp\n"
        "ENDPROCEDURE \n";
    return false;
}

static bool test__example__file_handling_operations(void) {
    char program[] =
        "DECLARE LineOfText : STRING\n"
        "OPENFILE FileA.txt FOR READ\n"
        "OPENFILE FileB.txt FOR WRITE\n"
        "WHILE NOT EOF(FileA.txt) DO\n"
        "   READFILE FileA.txt, LineOfText\n"
        "   IF LineOfText = \"\"\n"
        "       THEN\n"
        "           WRITEFILE FileB.txt, \"-------------------------\"\n"
        "       ELSE\n"
        "           WRITEFILE FILEB.txt, LineOfText\n"
        "   ENDIF\n"
        "ENDWHILE\n"
        "CLOSEFILE FileA.txt\n"
        "CLOSEFILE FileB.txt\n";
    return false;
}

static bool test__example__handling_random_files(void) {
    char program[] =
        "TYPE Student\n"
        "DECLARE Surname : STRING\n"
        "DECLARE FirstName : STRING\n"
        "DECLARE DateOfBirth : DATE\n"
        "DECLARE YearGroup : INTEGER\n"
        "DECLARE FormGroup : CHAR\n"
        "ENDTYPE\n"
        "\n"
        "DECLARE Pupil : Student\n"
        "DECLARE NewPupil : Student\n"
        "DECLARE Position : INTEGER\n"
        "NewPupil.Surname  \"Johnson\"\n"
        "NewPupil.Firstname  \"Leroy\"\n"
        "NewPupil.DateOfBirth  02/01/2005\n"
        "NewPupil.YearGroup  6\n"
        "NewPupil.FormGroup  ꞌAꞌ\n"
        "OPENFILE StudentFile.Dat FOR RANDOM\n"
        "FOR Position = 20 TO 10 STEP -1\n"
        "   SEEK StudentFile.Dat, Position\n"
        "   GETRECORD StudentFile.Dat, Pupil\n"
        "   SEEK StudentFile.Dat, Position + 1\n"
        "   PUTRECORD StudentFile.Dat, Pupil\n"
        "ENDFOR\n"
        "SEEK StudentFile.Dat, 10\n"
        "PUTRECORD StudentFile.Dat, NewPupil\n"
        "CLOSEFILE StudentFile.dat\n";
    return false;
}

// ---------------------------------------------------------

int main(void) {
    CP_ADD_TEST(test__upper);
    CP_ADD_TEST(test__vm_guess_stmt_kind_from_first_word__DECLARE);
    CP_ADD_TEST(test__vm_exec_stmt__DECLARE);
    
    CP_ADD_TEST(test__example__comments);
    CP_ADD_TEST(test__example__variable_declarations);
    CP_ADD_TEST(test__example__CONSTANT_declarations);
    CP_ADD_TEST(test__example__assignments);
    CP_ADD_TEST(test__example__array_declaration);
    CP_ADD_TEST(test__example__using_arrays);
    CP_ADD_TEST(test__example__assigning_an_array);
    CP_ADD_TEST(test__example__A_statement_should_not_however_refer_to_a_group_of_array_elements_individually);
    CP_ADD_TEST(test__example__assigning_a_group_of_array_elements);
    CP_ADD_TEST(test__example__declaration_of_custom_type);
    CP_ADD_TEST(test__example__using_custom_types);
    CP_ADD_TEST(test__example__INPUT_and_OUTPUT_statements);
    CP_ADD_TEST(test__example__nested_IF_statements);
    CP_ADD_TEST(test__example__formatted_CASE_statement);
    CP_ADD_TEST(test__example__nested_FOR_loops);
    CP_ADD_TEST(test__example__REPEAT_UNTIL_statement);
    CP_ADD_TEST(test__example__WHILE_loop);
    CP_ADD_TEST(test__example__use_of_procedures_with_and_without_parameters);
    CP_ADD_TEST(test__example__definition_and_use_of_a_function);
    CP_ADD_TEST(test__example__passing_parameters_by_reference);
    CP_ADD_TEST(test__example__file_handling_operations);
    CP_ADD_TEST(test__example__handling_random_files);

    CP_RUN_TESTS();

    return 0;
}
