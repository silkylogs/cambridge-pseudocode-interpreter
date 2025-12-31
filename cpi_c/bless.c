#include "bless.h"
#include <assert.h>

#define assert_stringslice(str) assert(str.start != NULL);

static size_t lesser(size_t a, size_t  b) { return (a < b) ? a : b; }

//   foobar   -> foobar
// ((foobar)) -> foobar
// (foo(bar)) -> foo(bar)
// ((foo)bar) -> (foo)bar
static struct StringSlice strip_outermost_parens(struct StringSlice str) {
	assert_stringslice(str);

	char *it;
	size_t lparen_cnt = 0;
	size_t rparen_cnt = 0;
	size_t least = 0;

	for (it = str.start; it <= str.start + str.len; it += 1) {
		if (*it == '(') lparen_cnt += 1;
		else break;
	}

	for (it = str.start + str.len - 1; it > str.start; it -= 1) {
		if (*it == ')') rparen_cnt += 1;
		else break;
	}

	least = lesser(lparen_cnt, rparen_cnt);
	str.start += least;
	str.len -= least;

	return str;
}

// ((...) + (...) * (...) - (...))
// If not found, return NULL
static char* find_topmost_leastpred_op_loc(struct StringSlice str) {
	assert_stringslice(str);

	struct StringSlice s = strip_outermost_parens(str);
	char *top_it = NULL;

	char *it = s.start;
	while (it < s.start + s.len) {
		if (*it == '(') {
			it = corresp_rparen_addr(it) + 1;
		} else if (*it == '+' || *it == '-') {
			top_it = it;
			it += 1;
		}
	}

	it = s.start;
	while (it < s.start + s.len) {
		if (*it == '(') {
			it = corresp_rparen_addr(it) + 1;
		} else if (*it == '*') {
			top_it = it;
			it += 1;
		}
	}

	it = s.start;
	while (it < s.start + s.len) {
		if (*it == '(') {
			it = corresp_rparen_addr(it) + 1;
		} else if (*it == '/') {
			top_it = it;
			it += 1;
		}
	}

	return top_it;
}

// "... <whitespace> foo <whitespace> +" -> foo
// "... <whitespace> (foo) <whitespace> +" -> (foo)
static struct StringSlice str_lt_term(char *op_loc) {

}

static struct StringSlice str_rt_term(char *op_loc) {

}

// (a + b) * c
// (...) * c
// (...) + (...) * (...)
// (...)
double eval_expr(struct StringSlice str) {
	// Find topmost operator or number. Ignore parenthesized terms.
	char *op_loc = find_topmost_leastpred_op_loc(str);
	if (op_loc_found(op_loc)) {
		struct StringSlice str_lhs = str_lt_term(op_loc);
		struct StringSlice str_rhs = str_rt_term(op_loc);

		double lhs = eval_expr(str_lhs);
		double rhs = eval_expr(str_rhs);

		char op = op_kind(op_loc);

		return eval_op(lhs, op, rhs);
	} else {
		// Strip parens, parse number within.
		struct StringSlice num_str = strip_outermost_parens(str);
		return str_to_num(num_str); // NaN if not a number.
	}
}

// ---------------------------------------------------------------------------

// is a character in a set of chars?
// set is expected to be a c-string
bool char_in(char c, const char *set) {
	if ('\0' == *set) return true; // Vacuous truth

	while ('\0' != *set) {
		if (c == *set) return true;
		set += 1;
	}
	return false;
}


// see test for expected usage
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
	int32_t multiplicand = 0;
	size_t actual_number_len_chars = 0;
	const char *it = 0;
	bool negative = 0;

	negative = str[0] == '-' ? true : false;
	if (negative) {str += 1;}

	actual_number_len_chars = count_longest_sequence_of_char_in_set(str, sz, "0123456789");
	if (0 == actual_number_len_chars) {
		ret.val = 0;
		ret.res = false;
		return ret;
	}

	multiplicand = 1;
	for (it = str + actual_number_len_chars - 1; it > str - 1; it -= 1) {
		int32_t digit = *it - '0';
		ret.val += digit * multiplicand;
		multiplicand *= 10;
	}

	if (negative) {ret.val = -ret.val;}
	ret.res = true;
	return ret;
}

// Sz points to the last character of the string. Not null. Is a slice.
float ascii_string_to_float(char const *str, size_t sz) {
	size_t dot_idx = 0;
	float result = 0;
	size_t dot_count = 0;

	for (dot_idx = 0; dot_idx <= sz; ++dot_idx) {
		if (str[dot_idx] == '.') {
			dot_count++;
			break;
		}
	}

	if (dot_count == 1) {
		size_t i;
		float multiplier_of_ten;
		
		// Integer part
		multiplier_of_ten = 1;
		for (i = dot_idx - 1; i != 0-1; --i) {
			result += (str[i] - '0') * multiplier_of_ten;
			multiplier_of_ten *= 10;
		}

		// Fractional part
		multiplier_of_ten = 0.1;
		for (i = dot_idx + 1; i < sz; ++i) {
			result += (str[i] - '0') * multiplier_of_ten;
			multiplier_of_ten /= 10;
		}

		return result;
	} else return (float)0xCCCCcccc;
}

bool literal_shallow_equality(struct Literal lit1, struct Literal lit2) {
	switch (lit1.kind) {
		case ADT_INTEGER: {
			return lit1.kind == lit2.kind
				&& lit1.val.as_int32 == lit2.val.as_int32
				&& lit1.is_valid == lit2.is_valid;
		}
		case ADT_REAL: {
			return lit1.kind == lit2.kind
				&& lit1.val.as_float == lit2.val.as_float
				&& lit1.is_valid == lit2.is_valid;
		}
		case ADT_CHAR: {
			return lit1.kind == lit2.kind
				&& lit1.val.as_char == lit2.val.as_char
				&& lit1.is_valid == lit2.is_valid;
		}
		case ADT_STRING: {
			return lit1.kind == lit2.kind
				&& lit1.val.as_string == lit2.val.as_string
				&& lit1.is_valid == lit2.is_valid;
		}
		case ADT_BOOLEAN: {
			return lit1.kind == lit2.kind
				&& lit1.val.as_boolean == lit2.val.as_boolean
				&& lit1.is_valid == lit2.is_valid;
		}
		case ADT_DATE: {
			return lit1.kind == lit2.kind
				&& lit1.val.as_date.day == lit2.val.as_date.day
				&& lit1.val.as_date.month == lit2.val.as_date.month
				&& lit1.val.as_date.year == lit2.val.as_date.year
				&& lit1.is_valid == lit2.is_valid;
		}
	}
}

struct Literal try_parse_literal(const char *str, size_t sz) {
	struct Literal retval = { 0 };
	retval.is_valid = true;

	struct Int32Result int_result = ascii_string_to_int32(str, sz);
	// struct FloatResult float_result = ascii_string_to_float(str, sz);
	// ...

	if (int_result.res) {
		retval.kind = ADT_INTEGER;
		retval.val.as_int32 = int_result.val;
		retval.val = retval.val;
	} else if (false) {
		// ...
	} else {
		retval.is_valid = false;
	}

	return retval;
}

struct StatementTypeAndBounds get_statement(const char *ptr) {
	struct StatementTypeAndBounds ret = {0};

	return ret;
}
