#ifndef CP_SRC_ASCIISTRINGVIEW
#define CP_SRC_ASCIISTRINGVIEW

struct CpAsciiStringView {
  char *chars;
  ssize len;
};

// TODO: write test functions for these
struct CpAsciiStringView CpAsciiStringView_from_const_cstr(const char *cstr, ssize cstr_len);
struct CpAsciiStringView CpAsciiStringView_from_const_cstr(const char *cstr, ssize cstr_len) {
  return (struct CpAsciiStringView) {
    .chars = (char *)cstr,
    .len = cstr_len,
  };
}

#define CP_ASCIISTRINGVIEW_FROM_CONST_CSTR(CSTR) \
  CpAsciiStringView_from_const_cstr((CSTR), sizeof (CSTR))

// get atomic type name
bool cp_case_sensitive_word_match(struct CpAsciiStringView src,
			       ssize src_index,
			       struct CpAsciiStringView type_name);
bool cp_case_sensitive_word_match(struct CpAsciiStringView src,
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

bool cp_test_case_sensitive_word_match(void);
bool cp_test_case_sensitive_word_match(void) {
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
  CP_TEST_FUNCTION(false, cp_case_sensitive_word_match, src1, 0, type_name1);
  
  auto src2 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("DECLARE Test: BOOL");
  auto type_name2 = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("BOOL");
  CP_TEST_FUNCTION(true, cp_case_sensitive_word_match, src2, 14, type_name2);

  return all_ok;
}

#endif
