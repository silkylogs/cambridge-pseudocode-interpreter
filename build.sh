#!/bin/sh
CC="clang"
RELEASE_FLAGS="-Wall -Werror -Weverything -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O0 -Wsign-compare -Wlogical-op -Wtype-limits -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wformat-nonliteral -fdiagnostics-color=never -Werror=vla"
OUT_FILE="cpi"
SRC_FILES="main.c"
C_LANG_STD="-std=c23"

$CC $SRC_FILES -o $OUT_FILE $FLAGS $C_LANG_STD
