#include "tree.h"

TreeNode* makeNodeWithChildren(size_t mChildCount){
	TreeNode *newNode = (TreeNode*)malloc(sizeof(TreeNode));
	if(newNode == NULL){
		printf("tree.c/makeNodeWithChildren(): ");
		printf("Unable to create new node\n");
		return NULL;
	}

	// Create (childCount) number of children
	size_t nodesToMake = mChildCount;

	TreeNode **childPtrs = (TreeNode**)malloc(nodesToMake * sizeof(TreeNode*));
	if(childPtrs == NULL){
		printf("tree.c/makeNodeWithChildren(): ");
		printf("Unable to allocate memory for pointers ");
		printf("to %llu child nodes\n", nodesToMake);
		free(newNode);
		return NULL;
	}

	// Make all nodes point to null
	// as cleanup operation
	for(size_t i = 0; i < nodesToMake; i++){
		childPtrs[i] = NULL;
	}

	newNode->children = childPtrs;
	newNode->currentChildrenCount = 0;
	newNode->maxChildCount = nodesToMake;
	newNode->data = NULL;
	return newNode;
}

void addChildToNode(TreeNode *parent, const TreeNode *child){
	// Check if there is room for one more child
	if(parent->currentChildrenCount >= parent->maxChildCount){
		printf("tree.c/addChildToNode(): ");
		printf("Cannot add another child to node with ");
		printf("max capacity of %llu children\n", parent->maxChildCount);
		return;
	}

	// Then add the child at index (currentChildrenCount-1)
	parent->currentChildrenCount++;
	parent->children[parent->currentChildrenCount-1] = (TreeNode*)child;
	return;
}

void* getChildData(const TreeNode *node, size_t childIndex){
	// Verify if index is correct
	if(childIndex >= node->maxChildCount){
		printf("tree.c/getChildData(): ");
		printf("index %llu is incorrect ", childIndex);
		printf("(valid index range: 0 <= i <= %llu)\n", node->maxChildCount-1);
	}

	return node->children[childIndex]->data;
}
