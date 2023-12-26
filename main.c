#include <stdio.h>
#include <stdbool.h>

typedef signed long long int ssize;

#define CP_LOG(WARN_LEVEL, MSG) \
  printf("%s:%d:0: %s: %s\n", __FILE__, __LINE__, WARN_LEVEL, MSG)

#include "CpAsciiStringView.c"



//

bool cp_run_all_tests(void);
bool cp_run_all_tests(void) {
  bool all_ok = true;
  bool current_ok = true;

  //current_ok &= cp_test_internal_atomic_type_sizes();
  //all_ok &= current_ok;
  
  current_ok &= cp_test_case_sensitive_word_match(); // insert function here
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
