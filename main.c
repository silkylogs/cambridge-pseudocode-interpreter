#include <stdio.h>
#include <stdbool.h>

typedef signed long long int ssize;

#define CP_LOG(WARN_LEVEL, MSG) \
  printf("%s:%d:0: %s: %s\n", __FILE__, __LINE__, WARN_LEVEL, MSG)

#include "CpAsciiStringView.c"

// get atomic type name
bool cp_case_insensitive_word_match(struct CpAsciiStringView src,
			       ssize src_index,
			       struct CpAsciiStringView type_name);
bool cp_case_insensitive_word_match(struct CpAsciiStringView src,
			       ssize src_index,
			       struct CpAsciiStringView type_name)
{
  bool all_matches = true;

  // WRITE A BETTER MATCHING ALGORIGHTM
  for (ssize i = src_index; i < src.len; ++i) {
    all_matches &= (src.chars[i] == type_name.chars[i - src_index]);
  }
  
  return all_matches;
}

bool cp_test_case_insensitive_word_match(void);
bool cp_test_case_insensitive_word_match(void) {
  bool all_ok = true;
  typedef struct CpAsciiStringView T;

#define CP_TEST_FUNCTION(RETURN_WHAT, FUNC_NAME, ...)		\
  do {								\
    bool curr_ok = (RETURN_WHAT) == (FUNC_NAME)(__VA_ARGS__);	\
    if (!curr_ok) {						\
      printf("%s:%d:0: Warning: function %s(%s) failed "	\
	     "(should return %s, returned %s)\n",		\
	     __FILE__, __LINE__,				\
	     #FUNC_NAME, #__VA_ARGS__,				\
	     (RETURN_WHAT)? "true": "false",			\
	     (curr_ok)? "true": "false");			\
    }								\
    all_ok &= curr_ok;						\
  } while(false);

    
  auto src1 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("INTEGER64");
  auto type_name1 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("INTEGER");
  CP_TEST_FUNCTION(false, cp_case_insensitive_word_match, src1, 0, type_name1);
  
  auto src2 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("DECLARE Test: BOOL");
  auto type_name2 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("BOOL");
  CP_TEST_FUNCTION(true, cp_case_insensitive_word_match, src2, 14, type_name2);

  return all_ok;
}

//

bool cp_run_all_tests(void);
bool cp_run_all_tests(void) {
  bool all_ok = true;
  bool current_ok = true;
  
  current_ok &= cp_test_case_insensitive_word_match(); // insert function here
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
