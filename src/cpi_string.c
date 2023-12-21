/*
 * Note to self: check the return value of memcpy() for errors,
 * should the need arise
 * Also stop worrying about freeing malloc()ed memory if it fails
 * 
 * second note to self: i cringe at this mess of a library
 */

#include "cpi_string.h"

#define LOCAL_WARNING(warningString) printf( "%s:%d:%s(): %s\n", __FILE__, __LINE__, __func__, warningString );

char* Cpi_String_getString(const Cpi_String *cs){
	char *emptyStr = EMPTY_STRING;
	if(cs == NULL) { return emptyStr; }
	else { return cs->contents; }
}

size_t Cpi_String_getStringLength(const Cpi_String *cs){
	return cs->length;
}

void Cpi_String_initNewStringObj(Cpi_String **cs, const char *newContents){
	if((*cs) != NULL) {
		LOCAL_WARNING("Attempt to reinitialize string");
		return;
	}

	size_t ncLen = strlen(newContents) + 1;

	(*cs) = malloc(sizeof(Cpi_String));
	if((*cs) == NULL){
		LOCAL_WARNING("Unable to allocate memory for string object");
		return;
	}

	(*cs)->contents = NULL;
	(*cs)->contents = malloc(ncLen);
	if((*cs)->contents == NULL) {
		LOCAL_WARNING(" Unable to allocate memory for string in string object");
		(*cs) = NULL;
		return;
	}

	memcpy((*cs)->contents, newContents, ncLen);
	(*cs)->length = strlen(newContents);
	return;
}

void Cpi_String_deleteStringObj(Cpi_String **cs){
	if((*cs) == NULL) { return; } // Do nothing if already null
	free((*cs)->contents);
	free(*cs);
	(*cs) = NULL;
	return;
}

void Cpi_String_copyStringObj(Cpi_String **cDest, const Cpi_String *cSrc){
	Cpi_String *temp = NULL;
	temp = malloc(sizeof(Cpi_String));
	if(temp == NULL) {
		LOCAL_WARNING("copyStringObj(): Unable to allocate memory for string object\n");
		return;
	}
	
	temp->contents = NULL;
	temp->contents = malloc(cSrc->length + 1);
	if(temp->contents == NULL) {
		LOCAL_WARNING("copyStringObj(): Unable to allocate memory for string in string object");
		return;
	}
	
	memcpy(temp->contents, cSrc->contents, cSrc->length + 1);
	temp->length = cSrc->length;

	Cpi_String_deleteStringObj(cDest);
	*cDest = temp;
	
	return;
}

// Derived functions
void Cpi_String_appendStringObjs(Cpi_String **original, const Cpi_String *cs){
	Cpi_String *temp = NULL;

	Cpi_String_initNewStringObj(&temp, "");
	if(temp == NULL) {
		printf("Additionally, cpistr.c/appendString(): failure to initialize new string");
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
		//printf("cpistr.c/appendStringObjs(): unable to reallocate memory for new string in string object");
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

	Cpi_String_deleteStringObj(original);
	*original = temp;
	return;
}

// Assumes that the return value of the function is a pointer pointing to null
// if not, do not forget to delete the string object's data beforehand
// Returns rightmost x characters from ThisString
Cpi_String* Cpi_String_right(const Cpi_String *ThisString, size_t x){
	Cpi_String *str = NULL;

	if(x <= 0) {
		Cpi_String_initNewStringObj(&str, "");
		return str;
	}
	else if(x >= ThisString->length){
		return (Cpi_String*)(ThisString);
	}
	else{
		size_t leftMostX = ThisString->length - x;
		char *rightMost = NULL;
		
		rightMost = malloc(leftMostX);
		if(rightMost == NULL) {
			printf("cpistr.c/right(): unable to allocate memory for rightmost %llu characters\n", x);
			Cpi_String_initNewStringObj(&str, "");
			return str;
		}
		
		memcpy(rightMost, ThisString->contents + leftMostX, x + 1);
		Cpi_String_initNewStringObj(&str, rightMost);
		return str;
	}
}

// Returns string of length y starting at position x from ThisString
Cpi_String* Cpi_String_mid(const Cpi_String *ThisString, const size_t x, const size_t y){
	Cpi_String *str = NULL;

	#define RETURN_NULL Cpi_String_initNewStringObj(&str, ""); return str;
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
	Cpi_String_initNewStringObj(&str, midChars);
	return str;
}

char Cpi_String_ucase(const char c){
	char out = c;
	if(out >= 'a' && out <= 'z') { out -= 32; }
	return out;
}

char Cpi_String_lcase(const char c){
	char out = c;
	if(out >= 'A' && out <= 'Z') { out += 32; }
	return out;
}
