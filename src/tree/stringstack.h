#ifndef STRINGSTACK_H
#define STRINGSTACK_H

#include <stdio.h>
#include <stdlib.h>

/*
 * Usage instructions:
 * - To create the stack, create a variable of type (StackAtom*)
 * - To add an element to the stack, assign the output of insert()
 *   to the stack pointer
 * - To pop the top of the stack ensure that the return value
 *   is the popped number, and that input is of type (StackAtom**)
*/


typedef struct StackAtom{
	char* data;
	struct StackAtom *next;
} StackAtom;

/* -- Base functions -- */

void stackPrint(const StackAtom *top);
StackAtom* stackInsert(StackAtom *current, const char* value);
StackAtom* stackPop(StackAtom **current);
size_t stackCount(const StackAtom* top);


/* -- Derived functions -- */

StackAtom* stackDupTop(StackAtom *top);

#endif
