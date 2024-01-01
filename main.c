#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef int64_t ssize;

#include "CpInternalLoggingUtils.c"
#include "CpAsciiStringView.c"
#include "CpTypeManager.c"
//#include "CpVariableManager.c"

bool cp_test_internal_atomic_type_sizes(void);
bool cp_test_internal_atomic_type_sizes(void) {
    return sizeof (ssize) == 8;
}

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

bool cp_run_all_tests(void);
bool cp_run_all_tests(void) {
    bool all_ok = true;
    
    all_ok &= CP_ASSERT(cp_test_internal_atomic_type_sizes());
    all_ok &= CP_ASSERT(cp_test_CpAsciiStringView_from_const_cstr());
    all_ok &= CP_ASSERT(cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked());
    all_ok &= CP_ASSERT(cp_test_CpTypeManager());
    /*all_ok &= CP_ASSERT(cp_test_CpVariableManager_push_var_to_stack_unchecked());*/
    
    return all_ok;
}

int main(void) {
    if (!cp_run_all_tests()) {
	CP_LOG("Error", "One or more tests returned failure");
	return -1;
    }

    CP_LOG("Info", "Program exited normally");
    return 0;
}
