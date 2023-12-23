
#ifndef CP_SRC_ASCIISTRINGVIEW
#define CP_SRC_ASCIISTRINGVIEW

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

#endif
