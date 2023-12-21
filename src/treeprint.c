#include "treeprint.h"

/* Commented out but not deleted as a prime example to follow YAGNI
 * instead of DRY
 * #define nullCheck(pointer, message) \
 *	if(pointer == NULL){ printf("%s\n", message); }
*/

/*
 * STATIC VARIABLES ARE A FOOTGUN IF YOU EVER INTEND TO USE THE FUNCTION AGAIN
 * Todo: either rewrite everything without using them
 * or find a way to reset them (like possibly furthestXExtent()?)
 */

size_t newLineCount(const char* string){
	if(string == NULL) {
		printf("treeprint.c/newLineCount(): ");
		printf("Warning: input to function is null, returning 0\n");
		return 0;
	}

	size_t count = 0;
	char* cp = (char*)string;
	
	while(*cp != '\0'){
		if(*cp == '\n'){ count++; }
		cp++;
	}

	return count;
}

size_t treeElementCount(const TreeNode* root){
	size_t acc = 0;

	if(root == NULL){ return 0; }
	for(size_t i = 0; i < root->currentChildrenCount; i++){ 
		acc += treeElementCount(root->children[i]);
	}

	return acc + 1;
}


// It produces correct results every time its called
// and i have no idea why
size_t furthestXExtent(const TreeNode* root){
	static size_t furthestExtent = 0;
	static size_t levelIndex = 0;

	if(root != NULL){
		size_t currentExtent;

		if(root->data == NULL) {
			printf("treeprint.c/furthestXExtent(): ");
			printf("Info: element of node points to null, skipping\n");
			currentExtent = levelIndex * 2;
		}
		else{
			//printf("Subject \"%s\" is at level %ld\n", root->data, levelIndex);
			currentExtent = strlen(root->data) + levelIndex * 2;
		}

		// Maybe replace <= with < to stop unnessecary assinging?
		if(furthestExtent < currentExtent){
			furthestExtent = currentExtent;
		}

		for(size_t i = 0; i < root->currentChildrenCount; i++){
			levelIndex++;
			furthestExtent = furthestXExtent(root->children[i]);
			levelIndex--;
		}
	}

	return furthestExtent;
}

size_t sumOfAllNewlinesInTree(const TreeNode* root){
	size_t currentNewLines = 0;

	if(root != NULL){
		if(root->data == NULL){
			printf("treeprint.c/sumOfAllNewlinesInTree(): ");
			printf("Info: element of node points to null, skipping\n");
		}
		else{
			// Even if string has only a null terminator it will
			// be considered as having a newline
			currentNewLines = newLineCount(root->data);
			currentNewLines++;
		}

		for(size_t i = 0; i < root->currentChildrenCount; i++){
			currentNewLines += sumOfAllNewlinesInTree(root->children[i]);
		}
	}

	return currentNewLines;
}


// Todo: remove origin support becuase
// a) the function ends up doing more than 1 thing
// b) if translation is sorely needed then create another function to
//    translate coordinates around
Coord2d* markCoordsX(const TreeNode *root, Coord2d* coordinateArray, size_t originX){
	static size_t functionCallCount = 0;
	
	// pretraversal used here
	if(root != NULL){
		//printf("---Marking x coordinate only for data \"%s\" (%ld,%ld)\n", root->data, originX, 0);
		coordinateArray[functionCallCount].x = originX;

		for(size_t i = 0; i < root->currentChildrenCount; i++){
			functionCallCount++;

			// Each child will be two spaces to the right of the parent
			coordinateArray = markCoordsX(root->children[i], coordinateArray, originX + 2);
		}
	}

	return coordinateArray;
}




char* create2dBuffer(size_t bufferSize){
	// usage: arr[y * width + x]
	// also consider using memset() to initialize

	char* buffer = malloc(sizeof(char) * bufferSize);
	if(buffer == NULL) {
		printf("treeprint.c/create2dStringBuffer(): ");
		printf("Error: Unable to create buffer of size %ld\n", bufferSize);
		return NULL;
	}

	return buffer;
}

void print2dBuffer(const char* buffer, size_t width, size_t height){
	for(size_t y = 0; y < height; y++){
		// print the first (width) characters of buffer,
		// starting from the address pointed to by (y*width)
		// where buffer is the base address
		printf("%.*s\n", width, buffer + (y*width));

		// the result is faster execution than
		// printing each character out, which can take a long time
		// when buffer is large
	}
}


StackAtom* preOrderStringsOntoStack(const TreeNode* root, StackAtom* stringStack){
	//static StackAtom* stringStack;

	if(root != NULL){
		// Allocate memory for copying over the string in the current node
		size_t stringLength = strlen(root->data) + 1;
		char* newString = malloc(sizeof(char) * stringLength);

		// Copy the string in the tree to the array
		memcpy(newString, root->data, stringLength + 1);
		newString[stringLength] = '\0';

		// Push the new string into the stack
		stringStack = stackInsert(stringStack, newString);


		// Do the same for all the children of this node
		for(size_t i = 0; i < root->currentChildrenCount; i++){
			stringStack = preOrderStringsOntoStack(root->children[i], stringStack);
		}
	}

	return stringStack;
}

// There is no way to know (yet) if input is an empty head
// as empty heads dont point to null
// Destroys the stack while in use, so better make a copy
char** stackToStringArray(StackAtom* strStkHead){
	if(strStkHead == NULL){
		printf("treeprint.c/stackToStringArray(): ");
		printf("null input detected\n");
		return NULL;
	}

	size_t strCnt = stackCount(strStkHead);

	// Extra 1 for null terminator for easier deallocation
	char** strArr = malloc(sizeof(char*) * strCnt + 1);
	strArr[strCnt] = NULL;

	// Not using size_t for indexing here because
	// terminating condition will never reach
	for(long long int i = strCnt - 1; i >= 0; i -= 1){

		// Make a new string
		size_t newStrLen = strlen(strStkHead->data);
		char* newString = malloc(newStrLen + 1);

		if(newString == NULL) {
			// Note to self: exit instead of letting it crash into obliviion
			printf("treeprint.c/stackToStringArray(): ");
			printf("Unable to allocate data for new string, expect memory leaks\n");
			return NULL;
		}

		// Copy over string in stack to new string
		memcpy(newString, strStkHead->data, newStrLen + 1);

		// And hence, into the string array
		strArr[i] = newString;

		strStkHead = stackPop(&strStkHead);
	}

	return strArr;
}


void freeStringArray(char** strArray){

	// Free all its elements
	size_t i = 0;
	while(strArray[i] != NULL){
		//printf("Freeing %s\n", strArray[i]);
		free(strArray[i]);
		i++;
	}

	// Free the array itself
	free(strArray);

	return;
}

// do actually _test_ this function, will you?
// and make originY actually functional
/*Coord2d* markCoordsY(const char** stringsInTree, Coord2d* coordinateArray, size_t originY){
	size_t i = 0;

	while(stringsInTree[i] != NULL){
		size_t newLinesInStr = newLineCount(stringsInTree[i]);
		if(newLinesInStr == 0) { newLinesInStr = 1; }

		coordinateArray[i].y = newLinesInStr + i;

		i++;
	}

	return coordinateArray;
}*/


// size_t outputLineCount can be determined from number of '\n's
// full algorithim involves creating a grid
// vertical space = sum of all newlines
// horizontal space = find largest (message + levelIndex * 2)
/*
 * how to print tree:
 * - find the string with the longest text (together with x offset from tree)
 * - find the sum of all newlines of all string elements in tree
 * - use the above two info to create a 2d buffer

 * - get preorder list of all string elements in tree
 * - get preorder x and y coords of where to print the text

 * - insert the string elements into the buffer with their respective coordinates
 * - insert the characters into buffer which makes the tree look like a tree
 * - print the contents of buffer
 
 * - free all memory used due to these operations
*/
void printTree(const TreeNode* root){
	size_t x = furthestXExtent(root);
	size_t y = sumOfAllNewlinesInTree(root);
	char* buffer = create2dBuffer(x * y); // mem created

	size_t numberOfElements = treeElementCount(root);
	StackAtom* stringStack = preOrderStringsOntoStack(root, stringStack);
	char** preOrderStrings = stackToStringArray(stringStack);
	// Coord2d* preOrderCoordinates = createCoord2dArray(numberOfElements) // mc
	// preOrderCoordinates = markCoordsX(root, preOrderCoordinates, 0) // starting from origin of buffer
	// preOrderCoordinates = markCoordsY(preOrderStrings, 0)

	// buffer = insertStrings(buffer, preOrderStrings, preOrderCoordinates)
	// buffer = insertTreeGraphics(buffer)
	print2dBuffer(buffer, x, y);

	free(buffer);
	freeStringArray(preOrderStrings);
	// free(preOrderCoordinates)
}
