#include "cpi.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//#include "lblcont.h"
//#include "vm.h"

// ---------------------------------------------------------

#define TEST(str) \
	this_test_name = str; \
	ok = true; 

#define TESTEND \
	printf("%3.3d: [%c] \"%s\"\n", test_no, (ok ? '+' : '-'), this_test_name); \
	all_test_ok &= ok; \
	test_no += 1;

#define EXPECT(expr) ok &= (expr);

int main(void) {
	 char *this_test_name = NULL;
	 bool ok = 0; // Did this test pass?
	 bool all_test_ok = true;
	 int test_no = 1;


	{
		TEST("Comments");

		CpiData d = {0};
		cpi_exec_file(&d, "../examples/eg_comments.txt");
		EXPECT(d.ok);

		TESTEND;
	} 

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


	 printf("\n");
	 printf("Key:\n  [+] Test succeeded\n  [-] Test failed\n");
	 printf("Summary: %s\n", all_test_ok ? "All tests passed" : "One or more tests failed");
	 return 0;
	}


