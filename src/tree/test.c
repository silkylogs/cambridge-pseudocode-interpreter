#include <stdio.h>
#include "tree.h"
#include "treeprint.h"

// Miscellaneous tests
#define TREEELEMCNT	0
#define LONGESTEXTENTS	0
#define BUFFER2D	0

// Why does only one of these work at a time?
// Answer: static counting variables
#define PREORDERSTRS	1 // problem here?
#define MARKPOSITIONS	0

// Tests for the tree structure
// and their basic functionality
#define MAKENODE	0
#define CHILDCOUNT	0
#define ADDCHILD	0
#define CHILDDATA	0



TreeNode* buildDummySubjectTree(){
	TreeNode* subjects	= makeNodeWithChildren(2);
	
	TreeNode* math		= makeNodeWithChildren(2);
	TreeNode* stats		= makeNodeWithChildren(0);
	TreeNode* pureMaths	= makeNodeWithChildren(0);
	
	TreeNode* science	= makeNodeWithChildren(3);
	TreeNode* physics	= makeNodeWithChildren(0);
	TreeNode* chemistry	= makeNodeWithChildren(0);

	TreeNode* biology	= makeNodeWithChildren(2);
	TreeNode* biostatistics	= makeNodeWithChildren(0);
	TreeNode* biochemistry	= makeNodeWithChildren(0);


	addChildToNode(subjects, math);
	addChildToNode(subjects, science);

	addChildToNode(math, stats);
	addChildToNode(math, pureMaths);

	addChildToNode(science, chemistry);
	addChildToNode(science, physics);
	addChildToNode(science, biology);

	addChildToNode(biology, biostatistics);
	addChildToNode(biology, biochemistry);

	subjects->data		= "subjects"	;

	math->data 		= "math"	;
	stats->data		= "statstics"	;
	pureMaths->data		= "pure\nmaths"	; // deliberate newline for stress test

	science->data		= "science"	;
	physics->data		= "physics"	;
	chemistry->data		= "chemistry"	;
	//chemistry->data = NULL; // for stress testing purposes

	biology->data		= "biology" 	;
	biostatistics->data	= "biostats"	;
	biochemistry->data	= "biochem"	;

	return subjects;	
}

int main(){

	if(TREEELEMCNT)
	{
		TreeNode* subjects = buildDummySubjectTree();
		printf("number of subjects: %ld\n", treeElementCount(subjects));
	}

	if(LONGESTEXTENTS){
		TreeNode* subjects = buildDummySubjectTree();

		printf("Longest x extent: %ld\n", furthestXExtent(subjects));
		printf("Longest y extent: %ld\n", sumOfAllNewlinesInTree(subjects));

		puts("again,");

		printf("Longest x extent: %ld\n", furthestXExtent(subjects));
		printf("Longest y extent: %ld\n", sumOfAllNewlinesInTree(subjects));
	}

	if(BUFFER2D)
	{
		size_t width  = 75;
		size_t height = 40;

		char* buffer = create2dBuffer(width * height);

		// fill buffer with data
		memset(buffer, 'g', width * height);

		// Print buffer
		print2dBuffer(buffer, width, height);

		puts("done");
		free(buffer);
	}

	if(PREORDERSTRS)
	{
		TreeNode* subjects = buildDummySubjectTree();

		// The reason for calling the functions twice
		// is to prove that the return value of the function
		// remains constant between calls for same arguements
		StackAtom* stack1 = preOrderStringsOntoStack(subjects, stack1);
		StackAtom* stack2 = preOrderStringsOntoStack(subjects, stack2);

		/*puts("Stack 1:");
		stackPrint(stack1);
		puts("\n");
		puts("Stack 2:");
		stackPrint(stack2);
		puts("\n");
		*/

		char** preStrings1 = stackToStringArray(stack1);
		char** preStrings2 = stackToStringArray(stack2);

		size_t i = 0;
		puts("Contents of prestrings1:");
		while(i < treeElementCount(subjects)){
			printf("%s;\n", preStrings1[i]);
			i++;
		}

		puts("\nContents of prestrings2");
		i = 0;
		while(i < treeElementCount(subjects)){
			printf("%s;\n", preStrings2[i]);
			i++;
		}

		freeStringArray(preStrings1);
		freeStringArray(preStrings2);
		puts("PREORDERSTRS test complete, no segfaults");
	}


	/*if(MARKPOSITIONS)
	{
		puts("Testing marking positions");
		TreeNode* subjects = buildDummySubjectTree();
		size_t subjectCount = treeElementCount(subjects);
		printf("Number of subjects: %ld\n", subjectCount);
		
		char** subjectStrs = malloc(sizeof(char*) * subjectCount);
		if(subjectStrs == NULL){ puts("unable to allocate subjectStrs"); }

		subjectStrs = preOrderStrings(subjects, subjectStrs);
		Coord2d* textCoords = createCoord2dArray(subjectCount);

		textCoords = markCoordsX(subjects, textCoords, 0);
		//textCoords = markCoordsY(subjectStrs, textCoords, 0);

		for(size_t i = 0; i < subjectCount; i++){
			//printf("(%ld,%ld): \"%s\"\n", textCoords[i].x, textCoords[i].y, subjectStrs[i]);
			printCoord( &textCoords[i] );
			printf(": \"%s\"\n", subjectStrs[i]);
		}
	}*/
	
	if(MAKENODE)
	{
		int dum1 = 69420;
		int dum2 = 69;
		int dum3 = 420;
		
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
		printf("At root: %d\n",       *(int*)(root->data));
		printf("At left leaf: %d\n",  *(int*)(root->children[0]->data));
		printf("At right leaf: %d\n", *(int*)(root->children[1]->data));
	}

	if(CHILDCOUNT)
	{
		int dum1 = 69420;
		int dum2 = 69;
		int dum3 = 420;
		TreeNode *n = makeNodeWithChildren(0xdead);
		size_t noChildren = n->maxChildCount;
		printf("number of children: %x\n", noChildren);
	}

	if(ADDCHILD)
	{
		int dum1 = 69420;
		int dum2 = 69;
		int dum3 = 420;
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
	}

	if(CHILDDATA)
	{
		int dum1 = 69420;
		int dum2 = 69;
		int dum3 = 420;
		
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
	}
	
	return 0;
}
