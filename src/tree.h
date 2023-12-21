/*
 * A tree data structure
 * with support for multiple children
 */
#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

// Todo: make support for parent
typedef struct TreeNode{
	void *data;

	size_t maxChildCount;
	size_t currentChildrenCount;
	struct TreeNode **children;
} TreeNode;

// note to self:
// if there must be children makers
// there conversely must be children destroyers
// ...kinda came out wrong but you surely know what i mean

TreeNode* makeNodeWithChildren(size_t mChildCount);
void addChildToNode(TreeNode *parent, const TreeNode *child);
void* getChildData(const TreeNode *node, size_t childIndex);


#endif
