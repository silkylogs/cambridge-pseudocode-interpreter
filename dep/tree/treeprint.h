#ifndef TREEPRINT_H
#define TREEPRINT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "coord2d.h"
#include "stringstack.h"

// todo: create a function as specified in treeprint.c
// then create the function body
// then test it vigourously after adding an entry in main()
// only then can you proceed

size_t newLineCount(const char* string);
size_t treeElementCount(const TreeNode* root);

size_t furthestXExtent(const TreeNode* root);
size_t sumOfAllNewlinesInTree(const TreeNode* root);

char* create2dBuffer(size_t bufferSize);
void print2dBuffer(const char* buffer, size_t x, size_t y);

StackAtom* preOrderStringsOntoStack(const TreeNode* root, StackAtom* stringStack);
char** stackToStringArray(StackAtom* strStkHead);
void freeStringArray(char** strArray);

Coord2d* markCoordsX(const TreeNode *root, Coord2d* coordinateArray, size_t originX);
//Coord2d* markCoordsY(const char** stringsInTree, Coord2d* coordinateArray, size_t originY);






#endif
