#ifndef CP_SRC_ASCIISTRINGVIEW
#define CP_SRC_ASCIISTRINGVIEW

struct CpAsciiStringView {
    char *chars;
    ssize len_including_nul;
};

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
    if (cstr == NULL) return false;
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
    CP_ASSERT(false == CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(NULL));
    CP_ASSERT(true == CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_CHECK(""));

    struct CpAsciiStringView test;
    test = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("This string has 25 chars");
    CP_ASSERT(test.len_including_nul == 25);
    CP_ASSERT(test.chars != NULL);
  
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



struct CpAsciiStringView_StringSubstringPair {
    struct CpAsciiStringView string;
    struct CpAsciiStringView substring;
};

bool CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(
    struct CpAsciiStringView_StringSubstringPair);

bool
CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(
    struct CpAsciiStringView_StringSubstringPair pair)
{
    struct CpAsciiStringView string = pair.string;
    struct CpAsciiStringView substring = pair.substring;
    
    if (substring.len_including_nul > string.len_including_nul) return false;
    for (ssize str_i = 0; str_i < string.len_including_nul; ++str_i) {
	if (string.chars[str_i] == substring.chars[0]) {
	    for (ssize sub_i = 0;
		 sub_i < substring.len_including_nul && sub_i < substring.len_including_nul;
		 ++sub_i)
	    {
		char str_char = string.chars[str_i + sub_i];
		char sub_char = substring.chars[sub_i];
		
		//printf("Comparing \'%c\' with \'%c\'\n", str_char, sub_char);
		if (sub_char == '\0') return true;
		if (str_char != sub_char) return false;
	    }
	}
    }
    return true;
}

#define CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(...)	\
    CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked( \
	(struct CpAsciiStringView_StringSubstringPair) {		\
	    __VA_ARGS__							\
	})

bool
cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(void);
bool
cp_test_CpAsciiStringView_match_case_sensitive_substring_in_string_unchecked(void) {
    bool all_ok = true;

    // Tests for intended usage
    all_ok &= CP_ASSERT(
        CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(
	    .string = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("DECLARE TestVar: BOOL"),
	    .substring = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("BOOL"))
	);

    all_ok &= CP_ASSERT(
        CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(
	    .substring = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("INTEGER"),
	    .string = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED("INTEGER64"))
	);

    // Improper usage: substring longer than original string
    all_ok &= CP_ASSERT(
        !CP_ASCIISTRINGVIEW_MATCH_CASE_SENSITIVE_SUBSTR_IN_STR_UNCHECKED(
	    .string = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED(
		"Obviously long test string"),
	    .substring = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR_UNCHECKED(
		"Obviously longer test string"))
	);

    return all_ok;
}

bool CpAsciiStringView_equals(struct CpAsciiStringView a, struct CpAsciiStringView b);
bool CpAsciiStringView_equals(struct CpAsciiStringView a, struct CpAsciiStringView b) {
    if (a.len_including_nul != b.len_including_nul) return false;
    ssize len = a.len_including_nul;

    return 0 == memcmp(a.chars, b.chars, len);
}

#endif
