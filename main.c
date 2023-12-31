#include <stdio.h>
#include <stdbool.h>

typedef signed long long int ssize;

#include "CpInternalLoggingUtils.c"
#include "CpAsciiStringView.c"


bool cp_test_internal_atomic_type_sizes(void);
bool cp_test_internal_atomic_type_sizes(void) {
    return sizeof (ssize) == 8;
}

bool cp_run_all_tests(void);
bool cp_run_all_tests(void) {
    bool all_ok = true;
    
    all_ok &= CP_ASSERT(cp_test_internal_atomic_type_sizes());
    all_ok &= CP_ASSERT(cp_test_CpAsciiStringView_from_const_cstr());
    all_ok &= CP_ASSERT(cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked());
    
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
