#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// ------------------------------------------------------------------

bool strEq(char *a, char *b) {
	return !strcmp(a, b);
}

bool isEmpty(char *s) {
	return strEq(s, "");
}

void appendChar(char **str, char c) {
	if (*str) {
		size_t zstrLen = strlen(*str) + 1;
		char *s = realloc(*str, zstrLen + 1);
		if (!s) {
			__debugbreak();
			exit(1);
		}
		
		s[zstrLen - 1] = c;
		s[zstrLen] = '\0';
		*str = s;
	} else {
		*str = malloc(2);
		if (!(*str)) {
			__debugbreak();
			exit(1);
		}
		*(*str + 0) = c;
		*(*str + 1) = '\0';
	}
}

// Leaky!
char *tryParseInteger(char *in) {
	if (isEmpty(in)) return "#ERR_EMPTY_INPUT";

	char *out = NULL;
	appendChar(&out, '#');

	char *p = in;
	if (*p == '-') appendChar(&out, 'N');
	else if (*p == '+') return "#ERR_INVALID_NUM";
	appendChar(&out, 'P');

	while (*p) {
		if (isdigit(*p)) {
			appendChar(&out, *p);
			p += 1;
		}
		else return "#ERR_INVALID_NUM";
	}

	appendChar(&out, '\0');
	return out;
}

// ------------------------------------------------------------------

// Executes one full lifetime of a program
char *eval(char *in) {
	if (strEq(in, "")) return "#ERR_EMPTY_INPUT";

	char *num = tryParseInteger(in);
	if (strEq(num, "#ERR_INVALID_NUM")) return "#ERR_INVALID_NUM";
	else return num;
}

// ------------------------------------------------------------------

void assEq(char *lhs, char *rhs) {
	if (!strEq(lhs, rhs)) {
		printf("Assertion failed: %s == %s\n", lhs, rhs);
		__debugbreak();
		exit(1);
	}
}

void assNe(char *lhs, char *rhs) {
	if (strEq(lhs, rhs)) {
		printf("Assertion failed: %s != %s\n", lhs, rhs);
		__debugbreak();
		exit(1);
	}
}

// ------------------------------------------------------------------

int main() {
	// As-is, leaky.
	//assEq(tryParseInteger("-42"), "#IN24");
	//assEq(tryParseInteger("+0"), "#ERR_INVALID_NUM");
	//assEq(tryParseInteger("78912foo"), "#ERR_INVALID_NUM");
	//assEq(tryParseInteger("1234567890"), "#IP0987654321");
	assEq(tryParseInteger("0"), "#IP0");
	assEq(tryParseInteger("0asdkjl"), "#ERR_INVALID_NUM");
	assEq(tryParseInteger(""), "#ERR_EMPTY_INPUT");

	assEq(eval(""), "#ERR_EMPTY_INPUT");

	return 0;
}
