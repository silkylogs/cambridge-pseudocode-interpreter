#include <stdio.h>
#include "cpistr.h"

int main(){
	CpiString *helloWorldStr = NULL;
	printf("initializing test string...\n");
	initNewStringObj(&helloWorldStr, "Hello world");
	printf("\"%s\"\n", getString(helloWorldStr));
	return 0;
}