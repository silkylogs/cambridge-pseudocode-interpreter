#include "stringstack.h"


/* -- Base functions -- */

/*
 * Given a pointer to a particular element, keep printing data
 * until a null pointer is reached 
 */
void stackPrint(const StackAtom *top){
	StackAtom *cp = (StackAtom*)top;
	if(cp == NULL) {
		puts("[NULL]");
		return;
	}
	
	while(cp != NULL){
		printf("[\"%s\"]\n", cp->data);
		cp = cp->next;
	}

	return;
}

/*
 * Given a pointer to the current head of the stack, inserts a new
 * integer to the stack
*/
StackAtom* stackInsert(StackAtom *current, const char* value){
	StackAtom *new_atom = (StackAtom*)(malloc(sizeof(StackAtom)));

	if(new_atom == NULL){
		printf("stack.c: stackInsert() failed to allocate memory\n");
		return NULL;
	}
	new_atom->data = (char*)value;
	new_atom->next = current;
	return new_atom;
}

/*
 * Given a pointer to the pointer to the top of the stack,
 * frees it and returns a pointer to its immediate successor.
*/
StackAtom* stackPop(StackAtom **current){
	if(*current == NULL){
		printf("stack.c: stackPop() attempted to pop a ");
		printf("null pointer. Perhaps there is nothing ");
		printf("on the top of the stack to pop off?\n");
		return NULL;
	}
	StackAtom *new_head = (*current)->next;
	free((*current)->data);
	free(*current);
	return new_head;
}

size_t stackCount(const StackAtom* top){
	StackAtom* cp = (StackAtom*)(top);
	size_t count = 0;

	while(cp != NULL){
		count++;
		cp = cp->next;
	}

	return count;
}



/* -- Derived functions -- */

/*
 * Given a pointer to the top of the stack, duplicates the top element
 * and returns a pointer to it
*/
StackAtom* stackDupTop(StackAtom *top){
	if(top == NULL){
		printf("stack.c: stackDupTop() attempted to duplicate a ");
		printf("null pointer. Maybe make sure the pointer is ");
		printf("pointing to a stack?\n");
		return NULL;
	}
	char* d_data = top->data;
	StackAtom *d_atom = stackInsert(top, d_data);
	if(d_atom == NULL){
		printf("Additional error from derived function stackDupTop(): ");
		printf("Unable to allocate memory\n");
		return NULL;
	}
	return d_atom;
}
/*int main(){
	StackAtom *head = NULL;
	head = insert(head, 9);
	head = insert(head, 6);
	head = insert(head, 345);

	printf("currently at top: %d\n", pop(&head));
	print_stack(head);
	head = delete_top(&head);
	print_stack(head);
	head = delete_top(&head);
	print_stack(head);
	head = delete_top(&head);
	pop(&head);
	print_stack(head);
	head = dup_top(head);

	head = insert(head, 6);
	head = dup_top(head);
	head = dup_top(head);
	print_stack(head);
	
	return 0;
}*/

