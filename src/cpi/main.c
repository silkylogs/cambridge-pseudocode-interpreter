#include <stdio.h>
#include "cpistr.h"
#include "cpi_iee.h"

int main(){
	CpiString *helloWorldStr = NULL;
	printf("initializing test string...\n");
	initNewStringObj(&helloWorldStr, "Hello world");
	printf("\"%s\"\n", getString(helloWorldStr));

	CPI_IEE_foo();
	return 0;
}