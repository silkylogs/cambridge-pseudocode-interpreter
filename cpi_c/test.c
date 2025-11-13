#include <assert.h>
#include "bless.h"

int main(void) {
	int32_t num;
	num = ascii_string_to_int32("249", 4).res;
	assert(249 == num);
	return 0;
}
