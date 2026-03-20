#include <stdio.h>
#include <stdbool.h>

// ------------------------------------------------------------------

bool strEq(char *a, char *b) {
	return 0 == strcmp(a, b);
}

char *tryParseNumber(char *in) {
	char *p = in;
	while (*p != '\0') {
		if (!isdigit(*p)) return "ERR_INVALID_NUM";
		p += 1;
	}
	return in;
}

// ------------------------------------------------------------------

// Executes one full lifetime of a program
char *eval(char *in) {
	if (strEq(in, "")) return "ERR_EMPTY_INPUT";

	char *num = tryParseNumber(in);
	if (strEq(num, "ERR_INVALID_NUM")) return "ERR_INVALID_NUM";
	else return num;
}

// ------------------------------------------------------------------

void assEq(char *lhs, char *rhs) {
	if (!(0 == strcmp(lhs, rhs))) {
		printf("Assertion failed: %s == %s\n", lhs, rhs);
		__debugbreak();
		exit(1);
	}
}

void assNe(char *lhs, char *rhs) {
	if (!(0 != strcmp(lhs, rhs))) {
		printf("Assertion failed: %s != %s\n", lhs, rhs);
		__debugbreak();
		exit(1);
	}
}

// ------------------------------------------------------------------

int main() {
	assEq(eval("-42"), "-42");

	assEq(eval("78912foo"), "ERR_INVALID_NUM");
	assEq(eval("789123487136582961"), "789123487136582961");
	assEq(eval("0"), "0");
	assNe(eval("0"), "ERR_INVALID_NUM");

	assEq(eval(""), "ERR_EMPTY_INPUT");

	return 0;
}
