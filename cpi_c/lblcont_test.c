#include "lblcont.c"

#include <stdio.h>

bool test_progmem(void) {
	size_t exp_sz = 0;
	size_t exp_cap = 50;

	struct ProgMem pm = progmem(50);

	assert(exp_sz == pm.sz && exp_cap == 
}
