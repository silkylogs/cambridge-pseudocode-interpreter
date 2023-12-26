#ifndef CP_SRC_ASCIISTRINGVIEW
#define CP_SRC_ASCIISTRINGVIEW

struct CpAsciiStringView {
    char *chars;
    ssize len_including_nul;
};

#define CP_REPORT(PRINTF_FMT_STR, ...)			\
    printf("%s:%d:0: Warning: " PRINTF_FMT_STR "\n",	\
	   __FILE__, __LINE__, __VA_ARGS__);

#define CP_REPORT_SINGLE(MSG)			\
    CP_REPORT(MSG "%s", "\n")

#define CP_ASSERT(EXPR)					\
    if(!((EXPR))) {					\
	CP_REPORT("Assertion failed: \"%s\"\n", #EXPR);	\
    }

// TODO: write test functions for these
struct CpAsciiStringView
CpAsciiStringView_from_const_cstr_unchecked(const char *cstr, ssize cstr_len);

struct CpAsciiStringView
CpAsciiStringView_from_const_cstr_unchecked(const char *cstr, ssize cstr_len) {
    return (struct CpAsciiStringView) {
	.chars = (char *)cstr,
	.len_including_nul = cstr_len,
    };
}

bool CpAsciiStringView_from_const_cstr_check(const char *cstr, ssize cstr_len);
bool CpAsciiStringView_check_well_formed(struct CpAsciiStringView str);

bool
CpAsciiStringView_from_const_cstr_check(const char *cstr, ssize cstr_len) {
    if (cstr == nullptr) return false;
    if (cstr_len < 0) return false;
    return true;
}

// Note: This exists in the first place because I thought some casting may be required
//       in order to call const_cstr_check
bool CpAsciiStringView_check_well_formed(struct CpAsciiStringView str) {
    return CpAsciiStringView_from_const_cstr_check(str.chars, str.len_including_nul);
}

#define CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(CSTR)			\
    CpAsciiStringView_from_const_cstr_check((CSTR), sizeof (CSTR))

#define CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED(CSTR)		\
    CpAsciiStringView_from_const_cstr_unchecked((CSTR), sizeof (CSTR))
  
bool cp_test_CpAsciiStringView_from_const_cstr(void);
bool cp_test_CpAsciiStringView_from_const_cstr(void) {
    // Test the macro checks for obvious defects  
    CP_ASSERT(false == CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(nullptr));
    CP_ASSERT(true == CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(""));

    struct CpAsciiStringView test;
    test = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("This string has 25 chars");
    CP_ASSERT(test.len_including_nul == 25);
    CP_ASSERT(test.chars != nullptr);
  
    return true;
}

struct CpAsciiStringView
CpAsciiStringView_substring_unchecked(struct CpAsciiStringView string, ssize idx1, ssize idx2);
struct CpAsciiStringView
CpAsciiStringView_substring_unchecked(struct CpAsciiStringView string, ssize idx1, ssize idx2) {
    if (idx1 > idx2) { ssize temp = idx1; idx1 = idx2; idx2 = temp; }

    struct CpAsciiStringView substr = {
	.chars = &string.chars[idx1],
	.len_including_nul = idx2 - idx1
    };

    return substr;
}


bool
CpAsciiStringView_substring_check(struct CpAsciiStringView string, ssize idx1, ssize idx2);
bool
CpAsciiStringView_substring_check(struct CpAsciiStringView string, ssize idx1, ssize idx2) {
    // Out of bounds check
    if (idx1 < 0 || idx1 >= string.len_including_nul) return false;
    if (idx2 < 0 || idx2 >= string.len_including_nul) return false;

    // Source string anomaly check
    if (!CpAsciiStringView_check_well_formed(string)) return false;

    return true;
}

bool CpAsciiStringView_\
match_case_sensitive_substring_in_string_\
unchecked(struct CpAsciiStringView string, struct CpAsciiStringView substring);

bool CpAsciiStringView_\
match_case_sensitive_substring_in_string_\
unchecked(struct CpAsciiStringView string, struct CpAsciiStringView substring) {
    for (ssize str_i = 0; str_i < string.len_including_nul; ++str_i) {
	if (string.chars[str_i] == substring.chars[0]) {
	    for (ssize sub_i = 0;
		 sub_i < substring.len_including_nul && sub_i < substring.len_including_nul;
		 ++sub_i)
	    {
		if (string.chars[str_i + sub_i] != substring.chars[sub_i]) return false;
	    }
	}
    }
    return true;
}

// get atomic type name
/*
  bool cp_case_sensitive_word_match(struct CpAsciiStringView src,
  ssize src_index,
  struct CpAsciiStringView type_name);
  bool cp_case_sensitive_word_match(struct CpAsciiStringView src,
  ssize src_index,
  struct CpAsciiStringView type_name)
  {
  bool all_matches = true;

  // WRITE A BETTER MATCHING ALGORIGHTM
  for (ssize i = src_index; i < src.len_including_nul; ++i) {
  all_matches &= (src.chars[i] == type_name.chars[i - src_index]);
  }
  
  return all_matches;
  }

  bool cp_test_case_sensitive_word_match(void);
  bool cp_test_case_sensitive_word_match(void) {
  bool all_ok = true;
  typedef struct CpAsciiStringView T;

  #define CP_TEST_FUNCTION(RETURN_WHAT, FUNC_NAME, ...)		\
  do {								\
  bool curr_ok = (RETURN_WHAT) == (FUNC_NAME)(__VA_ARGS__);	\
  if (!curr_ok) {						\
  CP_REPORT("function %s(%s) failed "			\
  "(should return %s, returned %s)\n",		\
  #FUNC_NAME, #__VA_ARGS__,			\
  (RETURN_WHAT)? "true": "false",			\
  (curr_ok)? "true": "false");			\
  }								\
  all_ok &= curr_ok;						\
  } while(false);

    
  auto src1 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("INTEGER64");
  auto type_name1 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("INTEGER");
  CP_TEST_FUNCTION(false, cp_case_sensitive_word_match, src1, 0, type_name1);
  
  auto src2 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("DECLARE Test: BOOL");
  auto type_name2 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("BOOL");
  CP_TEST_FUNCTION(true, cp_case_sensitive_word_match, src2, 14, type_name2);

  return all_ok;
  }
*/

#endif
