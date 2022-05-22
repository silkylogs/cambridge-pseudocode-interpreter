#ifndef COORD2D_H
#define COORD2D_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Coord2d{
	size_t x;
	size_t y;
} Coord2d;

Coord2d* createCoord2dArray(const size_t arraySize);
void printCoord(const Coord2d* coordinate);

#endif