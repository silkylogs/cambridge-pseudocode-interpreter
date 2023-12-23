#include <stdio.h>

typedef signed long long int ssize;

#include "CpAsciiStringView.c"

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
