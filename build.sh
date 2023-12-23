#!/bin/sh
CC="gcc"
RELEASE_FLAGS="-Wall -Werror -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -O0 -Wsign-compare -Wlogical-op -Wtype-limits -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wformat-nonliteral"
OUT_FILE="cpi"
SRC_FILES="main.c"
C_LANG_STD="-std=c99"

$CC $SRC_FILES -o $OUT_FILE $FLAGS $C_LANG_STD
