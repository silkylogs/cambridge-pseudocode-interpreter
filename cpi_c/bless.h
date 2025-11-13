#ifndef BLESS_H
#define BLESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


struct Int32Result {
	int32_t val;
	uint32_t res;
};

bool char_in(char c, const char *set);
size_t count_longest_sequence_of_char_in_set(const char *str, size_t sz, const char *set);
struct Int32Result ascii_string_to_int32(const char *str, size_t sz);

/*
TODO: write usage code first

enum Type { T_INTEGER, T_REAL, T_BOOLEAN, };

struct Lexeme {
	enum Type type;
	size_t id; // Index to context
	const char *substr;
	size_t substr_len;
};

struct Lexeme lex(const char *str);


#define MAX_VALUE_COUNT (1024)
struct BlessCtx {
	int32_t integers[MAX_VALUE_COUNT];
	float reals[MAX_VALUE_COUNT];
	bool booleans[MAX_VALUE_COUNT];
};
*/

#endif
