#include "coord2d.h"

Coord2d* createCoord2dArray(const size_t arraySize){
	Coord2d* cArr = malloc(sizeof(Coord2d) * arraySize);
	if(cArr == NULL) {
		printf("treePrint.c/createCoord2DArray: Unable to create array\n");
		return NULL;
	}
	return cArr;
}

void printCoord(const Coord2d* coordinate){
	if(coordinate == NULL){ 
		printf("(NULL, NULL)");
	}
	else{
		printf("(%ld,%ld)", coordinate->x, coordinate->y);
	}
	return;
}