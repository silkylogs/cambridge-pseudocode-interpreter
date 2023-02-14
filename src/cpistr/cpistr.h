#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CpiString{
	char *contents;
	size_t length;
} CpiString;

#define EMPTY_STRING "[null]"

char* getString(const CpiString *cs);
size_t getStringLength(const CpiString *cs);
void initNewStringObj(CpiString **cs, const char *newContents);
void deleteStringObj(CpiString **cs);
void copyStringObj(CpiString **cDest, const CpiString *cSrc);

// Derived function
void appendStringObjs(CpiString **original, const CpiString *cs);

// Pseudocode specific functions
CpiString* right(const CpiString *ThisString, size_t x);
CpiString* mid(const CpiString *ThisString, const size_t x, const size_t y);
char ucase(const char c);
char lcase(const char c);
