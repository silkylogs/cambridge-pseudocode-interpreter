#include <stdio.h>
#include "cpi_string.h"
#include "cpi_iee.h"

int main(){
	Cpi_String *helloWorldStr = 0;
	printf("initializing test string...\n");
	Cpi_String_initNewStringObj(&helloWorldStr, "Hello world");
	printf("\"%s\"\n", Cpi_String_getString(helloWorldStr));

	CPI_IEE_foo();
	return 0;
}