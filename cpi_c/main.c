#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "bless.h"

int main(void) {
	int32_t num;
	num = ascii_string_to_int32("249", 4).res;
	printf("%d\n", num);
	return 0;
}
