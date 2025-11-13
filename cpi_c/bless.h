#ifndef BLESS_H
#define BLESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>


struct Int32Result {
	int32_t val;
	uint8_t padding[3];
	bool res;
};

bool char_in(char c, const char *set);
size_t count_longest_sequence_of_char_in_set(const char *str, size_t sz, const char *set);
struct Int32Result ascii_string_to_int32(const char *str, size_t sz);

#endif
