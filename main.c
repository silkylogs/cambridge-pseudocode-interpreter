#include <stdio.h>
#include <stdbool.h>

typedef signed long long int ssize;

#define CP_LOG(WARN_LEVEL, MSG)						\
    printf("%s:%d:1: %s: %s\n", __FILE__, __LINE__, WARN_LEVEL, MSG)

#define CP_REPORT(PRINTF_FMT_STR, ...)			\
    printf("%s:%d:1: Warning: " PRINTF_FMT_STR "\n",	\
	   __FILE__, __LINE__, __VA_ARGS__);

#define CP_ASSERT(EXPR)					\
    if(!((EXPR))) {					\
	CP_REPORT("Assertion failed: \"%s\"\n", #EXPR);	\
    }

#define CP_ASSERT_SET(EXPR, BOOL)		\
    CP_ASSERT(EXPR);				\
    BOOL = (EXPR)

#include "CpAsciiStringView.c"


bool cp_test_internal_atomic_type_sizes(void);
bool cp_test_internal_atomic_type_sizes(void) {
    return sizeof (ssize) == 8;
}

bool cp_run_all_tests(void);
bool cp_run_all_tests(void) {
    bool all_ok = true;
    bool current_ok = true;

    CP_ASSERT_SET(cp_test_internal_atomic_type_sizes(), current_ok);
    all_ok &= current_ok;

    CP_ASSERT_SET(cp_test_CpAsciiStringView_from_const_cstr(), current_ok);
    all_ok &= current_ok;

    CP_ASSERT_SET(
	cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(), current_ok);
    all_ok &= current_ok;
    
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
