#include <stdio.h>
#include "tree.h"

#define MAKENODE	0
#define CHILDCOUNT	0
#define ADDCHILD	0
#define CHILDDATA	1

int dum1 = 69420;
int dum2 = 69;
int dum3 = 420;

int main(){
	#if MAKENODE
	TreeNode *root = makeNodeWithChildren(2);
	TreeNode *leftLeaf = makeNodeWithChildren(0);
	TreeNode *rightLeaf = makeNodeWithChildren(0);

	// idea: make a child assigner
	// which complains when a) index is out of bounds
	// or b) index is the forbidden last child
	// (note to self: we arent doing null children, this aint a linked list)
	root->children[0] = leftLeaf;
	root->children[1] = rightLeaf;
	root->children[2] = NULL;

	// Likewise, consider making a child data setter
	root->data = &dum1;
	root->children[0]->data = &dum2;
	root->children[1]->data = &dum3;

	// Likewise, also a child data getter
	printf("At root: %d\n",      *(int*)(root->data));
	printf("At left leaf: %d\n", *(int*)(root->children[0]->data));
	printf("At right leaf: %d\n", *(int*)(root->children[1]->data));
	#endif

	#if CHILDCOUNT
	TreeNode *n = makeNodeWithChildren(0xdead);
	size_t noChildren = n->maxChildCount;
	printf("number of children: %x\n", noChildren);
	#endif

	#if ADDCHILD
	TreeNode *root = makeNodeWithChildren(2);
	TreeNode *leftLeaf = makeNodeWithChildren(0);
	TreeNode *rightLeaf = makeNodeWithChildren(1);
	TreeNode *unwantedLeaf = makeNodeWithChildren(0);
	

	addChildToNode(root, leftLeaf);
	addChildToNode(root, rightLeaf);

	addChildToNode(root, unwantedLeaf);
	addChildToNode(leftLeaf, unwantedLeaf);
	addChildToNode(rightLeaf, unwantedLeaf);
	
	root->data = &dum1;
	root->children[0]->data = &dum2;
	root->children[1]->data = &dum3;

	printf("At root: %d\n",      *(int*)(root->data));
	printf("At left leaf: %d\n", *(int*)(root->children[0]->data));
	printf("At right leaf: %d\n", *(int*)(root->children[1]->data));
	#endif

	#if CHILDDATA
	TreeNode *d0 = makeNodeWithChildren(2);
	TreeNode *d1 = makeNodeWithChildren(1);
	TreeNode *d2 = makeNodeWithChildren(0);
	

	addChildToNode(d0, d1);
	addChildToNode(d1, d2);
	
	d0->data = &dum1;
	d0->children[0]->data = &dum2;
	d0->children[0]->children[0]->data = &dum3;

	printf("At d0: %d\n", *(int*)(d0->data));
	printf("At d1: %d\n", *(int*)(getChildData(d0, 0)));
	printf("At d2: %d\n", *(int*)(getChildData(d0->children[0], 0)));
	printf("Also at d2: %d\n", *(int*)(getChildData(d1, 0)));
	
	#endif
	return 0;
}
