/*
 * Note to self: check the return value of memcpy() for errors,
 * should the need arise
 * Also stop worrying about freeing malloc()ed memory if it fails
 */

#include "cpistr.h"


char* getString(const CpiString *cs){
	char *emptyStr = EMPTY_STRING;
	if(cs == NULL) { return emptyStr; }
	else { return cs->contents; }
}

size_t getStringLength(const CpiString *cs){
	return cs->length;
}

void initNewStringObj(CpiString **cs, const char *newContents){
	if((*cs) != NULL) {
		printf("cpistr.c/initNewStringObj(): Attempt to reinitialize string\n");
		return;
	}

	size_t ncLen = strlen(newContents) + 1;

	(*cs) = malloc(sizeof(CpiString));
	if((*cs) == NULL){
		printf("cpistr.c/initNewStringObj(): Unable to allocate memory for string object\n");
		return;
	}

	(*cs)->contents = NULL;
	(*cs)->contents = malloc(ncLen);
	if((*cs)->contents == NULL) {
		printf("cpistr.c/initNewStringObj(): Unable to allocate memory for string in string object\n");
		(*cs) == NULL;
		return;
	}

	memcpy((*cs)->contents, newContents, ncLen);
	(*cs)->length = strlen(newContents);
	return;
}

void deleteStringObj(CpiString **cs){
	if((*cs) == NULL) { return; } // Do nothing if already null
	free((*cs)->contents);
	free(*cs);
	(*cs) = NULL;
	return;
}

void copyStringObj(CpiString **cDest, const CpiString *cSrc){
	CpiString *temp = NULL;
	temp = malloc(sizeof(CpiString));
	if(temp == NULL) {
		printf("cpistr.c/copyStringObj(): Unable to allocate memory for string object\n");
		return;
	}
	
	temp->contents = NULL;
	temp->contents = malloc(cSrc->length + 1);
	if(temp->contents == NULL) {
		printf("cpistr.c/copyStringObj(): Unable to allocate memory for string in string object\n");
		return;
	}
	
	memcpy(temp->contents, cSrc->contents, cSrc->length + 1);
	temp->length = cSrc->length;

	deleteStringObj(cDest);
	*cDest = temp;
	
	return;
}

// Derived functions
void appendStringObjs(CpiString **original, const CpiString *cs){
	CpiString *temp = NULL;

	initNewStringObj(&temp, "");
	if(temp == NULL) {
		printf("Additionally, cpistr.c/appendString(): failure to initialize new string\n");
		return;
	}

	/*if(NULL == realloc(temp->contents, (*original)->length + cs->length + 2)){
		printf("cpistr.c/appendStringObjs(): unable to reallocate memory for new string in string object\n");
		free(temp);
		return;
	}*/
	free(temp->contents);
	temp->contents = NULL;
	temp->contents = malloc((*original)->length + cs->length + 2);
	if(temp->contents == NULL) {
		printf("cpistr.c/appendStringObjs(): unable to reallocate memory for new string in string object\n");
		free(temp);
		return;
	}


	temp->length = (*original)->length + cs->length;
	
	// copy the original string contents onto temp
	// then get a pointer to the end of the original c-string in the temp object
	// then memcpy() to that location
	memcpy(temp->contents, (*original)->contents, (*original)->length + 1);
	char *originalEnd = temp->contents + (*original)->length;
	memcpy(originalEnd, cs->contents, cs->length + 1);

	deleteStringObj(original);
	*original = temp;
	return;
}

// Assumes that the return value of the function is a pointer pointing to null
// if not, do not forget to delete the string object's data beforehand
// Returns rightmost x characters from ThisString
CpiString* right(const CpiString *ThisString, size_t x){
	CpiString *str = NULL;

	if(x <= 0) {
		initNewStringObj(&str, "");
		return str;
	}
	else if(x >= ThisString->length){
		return (CpiString*)(ThisString);
	}
	else{
		size_t leftMostX = ThisString->length - x;
		char *rightMost = NULL;
		
		rightMost = malloc(leftMostX);
		if(rightMost == NULL) {
			printf("cpistr.c/right(): unable to allocate memory for rightmost %llu characters\n", x);
			initNewStringObj(&str, "");
			return str;
		}
		
		memcpy(rightMost, ThisString->contents + leftMostX, x + 1);
		initNewStringObj(&str, rightMost);
		return str;
	}
}

// Returns string of length y starting at position x from ThisString
CpiString* mid(const CpiString *ThisString, const size_t x, const size_t y){
	CpiString *str = NULL;

	#define RETURN_NULL initNewStringObj(&str, ""); return str;
	if(x <= 0 || x > ThisString->length || y <= 0) { printf("pt\n"); RETURN_NULL }

	size_t maxY = (ThisString->length) - x + 1;
	size_t effectiveY = 0;
	char *midChars = NULL;
	
	if(y >= maxY) { effectiveY = maxY; }
	else { effectiveY = y; }
	
	midChars = malloc(effectiveY + 1);
	if(midChars == NULL) {
		printf("cpistr.c/mid(): unable to allocate memory for offset (%llu) characters\n", y);
		RETURN_NULL
	}

	memcpy(midChars, ThisString->contents + x - 1, effectiveY);
	initNewStringObj(&str, midChars);
	return str;
}

char ucase(const char c){
	char out = c;
	if(out >= 'a' && out <= 'z') { out -= 32; }
	return out;
}

char lcase(const char c){
	char out = c;
	if(out >= 'A' && out <= 'Z') { out += 32; }
	return out;
}
