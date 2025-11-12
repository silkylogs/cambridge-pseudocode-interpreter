#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// is a character in a set of chars?
// set is expected to be a c-string
bool char_in(char c, const char *set) {
	while ('\0' != *set) {
		if (c == *set) return true;
		set += 1;
	}
	return false;
}

struct Int32Result {
	int32_t val;
	bool res;
};

// 0001a returns 3, given set "01234567890"
// set is expected to be a c-string
size_t 
count_longest_sequence_of_char_in_set(const char *str, size_t sz, const char *set) {
	size_t count = 0;
	for (size_t i = 0; i < sz; ++i) {
		if (char_in(str[i], set)) {
			count += 1;
		} else {
			break;
		}
	}
	return count;
}

struct Int32Result
ascii_string_to_int32(const char *str, size_t sz) {
	struct Int32Result ret = {0};
	size_t i = 0;

	size_t actual_number_len_chars = count_longest_sequence_of_char_in_set(str, sz, "0123456789");
	if (0 == actual_number_len_chars) {
		ret.val = 0;
		ret.res	= false;
		return ret;
	}

	i = actual_number_len_chars;
	int32_t multiplicand = 1;
	while (i) {
		int32_t digit = str[i] - '0';
		ret.val += digit * multiplicand;
		multiplicand *= 10;
		i -= 1;	
	}

	ret.res = true;
	return ret;
}


int main() {
	int32_t num = ascii_string_to_int32("249", 4).res;
	printf("%d\n", num);
	return 0;
}
