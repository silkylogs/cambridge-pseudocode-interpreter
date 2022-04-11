#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CpiString{
	char *contents;
	size_t length;
} CpiString;

#define EMPTY_STRING "[null]"

char* getString(const CpiString *cs);
size_t getLength(const CpiString *cs);
void initNewStringObj(CpiString **cs, const char *newContents);
void deleteStringObj(CpiString **cs);
void copyStringObj(CpiString **cDest, const CpiString *cSrc);

// Derived function
void appendStringObjs(CpiString **original, const CpiString *cs);

// Pseudocode functions
CpiString* right(const CpiString *ThisString, size_t x);

/*String* mid(const String *str, const size_t x, const size_t y);
String* ucase(const String *str);
String* lcase(const String *str);*/
