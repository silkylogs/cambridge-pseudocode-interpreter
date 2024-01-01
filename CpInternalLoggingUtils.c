#ifndef CP_SRC_INTERNAL_LOGGING_UTILS
#define CP_SRC_INTERNAL_LOGGING_UTILS

#define CP_LOG(WARN_LEVEL, MSG)						\
    printf("%s:%d:1: %s: %s\n", __FILE__, __LINE__, WARN_LEVEL, MSG)

#define CP_REPORT(PRINTF_FMT_STR, ...)			\
    printf("%s:%d:1: Warning: " PRINTF_FMT_STR "\n",	\
	   __FILE__, __LINE__, __VA_ARGS__);

bool cp_assert_internal(bool b, const char *b_expression, const char *file, int line);
bool cp_assert_internal(bool b, const char *b_expression, const char *file, int line) {
    if (!b)
	printf("%s:%d:1: Assertion failure:\n"
	       "---------- Expression starts here: ----------\n"
	       "%s\n"
	       "---------- Expression ends here.   ----------\n\n",
	       file, line, b_expression);
    return b;
}

#define CP_ASSERT(EXPR) cp_assert_internal((EXPR), #EXPR, __FILE__, __LINE__)

#define CP_ASSERT_SET(EXPR, BOOL)		\
    CP_ASSERT(EXPR);				\
    BOOL = (EXPR)

#endif
