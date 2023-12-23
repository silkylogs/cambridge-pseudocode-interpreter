#include <stdio.h>

typedef signed long long int ssize;
struct CpAsciiStringView {
  char *chars;
  ssize len;
};

struct CpAsciiStringView CpAsciiStringView_from_const_cstr(const char *cstr, ssize cstr_len);
struct CpAsciiStringView CpAsciiStringView_from_const_cstr(const char *cstr, ssize cstr_len) {
  return (struct CpAsciiStringView) {
    .chars = (char *)cstr,
    .len = cstr_len,
  };
}

#define CP_ASCIISTRINGVIEW_FROM_CONST_CSTR(CSTR) \
  CpAsciiStringView_from_const_cstr((CSTR), sizeof (CSTR))

/*
struct CpToken {
  enum CpTokenType {
    CP_TOKEN_TYPE_ATOMIC_TYPE,
  } token_type;
};
*/

int main(void){
  struct CpAsciiStringView str = CP_ASCIISTRINGVIEW_FROM_CONST_CSTR("Hello, world!\n");
  printf("%s", str.chars);
  return 0;
}
