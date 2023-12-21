#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cpi_String{
	char *contents;
	size_t length;
} Cpi_String;

#define EMPTY_STRING "[null]"

char* Cpi_String_getString( const Cpi_String *cs );
size_t Cpi_String_getStringLength(const Cpi_String *cs);
void Cpi_String_initNewStringObj(Cpi_String **cs, const char *newContents);
void Cpi_String_deleteStringObj(Cpi_String **cs);
void Cpi_String_copyStringObj(Cpi_String **cDest, const Cpi_String *cSrc);

// Derived function
void Cpi_String_appendStringObjs(Cpi_String **original, const Cpi_String *cs);

// Pseudocode specific functions
Cpi_String* Cpi_String_right(const Cpi_String *ThisString, size_t x);
Cpi_String* Cpi_String_mid(const Cpi_String *ThisString, const size_t x, const size_t y);
char Cpi_String_ucase(const char c);
char Cpi_String_lcase(const char c);
