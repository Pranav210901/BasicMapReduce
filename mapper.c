#include "mapper.h"

static intermediateDS * DSroot = NULL;

// combined value list corresponding to a word <1,1,1,1....>
valueList *createNewValueListNode(char *value){							// create new value node, <value, NULL>
	valueList *newNode = (valueList *)malloc (sizeof(valueList));
	strcpy(newNode -> value, value);
	newNode -> next = NULL;
	return newNode;
}

// insert new count to value list
valueList *insertNewValueToList(valueList *root, char *count){			// inserting new value node
	valueList *tempNode = root;

	if(root == NULL)
		return createNewValueListNode(count);							// first occurence, create new value list node +1	

	while(tempNode -> next != NULL)										// else search through until NULL, create new node +1
		tempNode = tempNode -> next;
	tempNode -> next = createNewValueListNode(count);

	return root;
}

// free value list
void freeValueList(valueList *root) {
	if(root == NULL) return;

	valueList *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		free(tempNode);
	}
}

// create <word, value list>
intermediateDS *createNewInterDSNode(char *word, char *count){
	intermediateDS *newNode = (intermediateDS *)malloc (sizeof(intermediateDS));
	strcpy(newNode -> key, word);
	newNode -> value = NULL;
	newNode -> value = insertNewValueToList(newNode -> value, count);
	newNode -> next = NULL;
	return newNode;
}

// insert or update a <word, value> to intermediate DS
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count){
	intermediateDS *tempNode = root;
	if(root == NULL)												// if empty, create new node
		return createNewInterDSNode(word, count);

	while(tempNode -> next != NULL) {								// else search node 
		if(strcmp(tempNode -> key, word) == 0){						// if key is existing, update valueList +1
			tempNode -> value = insertNewValueToList(tempNode -> value, count);
			return root;
		}
		tempNode = tempNode -> next;								// else keep searching
	}

	if(strcmp(tempNode -> key, word) == 0) {						// at the end of the list, update node value +1?
		tempNode -> value = insertNewValueToList(tempNode -> value, count);
	} 
	
	else {
		tempNode -> next = createNewInterDSNode(word, count);		// end of list, create new node <word, count [1]>
	}

	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeInterDS(intermediateDS *root) {
	if(root == NULL) return;

	intermediateDS *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		freeValueList(tempNode -> value);
		free(tempNode);
	}
}

// emit the <key, value> into intermediate DS 
void emit(char *key, char *value) {
	DSroot = insertPairToInterDS (DSroot, key, value);
}

// map function
void map(char *chunkData){
	int i = 0;
	char * buffer;
	char * count = "1";

	while ((buffer = getWord(chunkData, &i)) != NULL){
		emit (buffer, count);
	}

	// you can use getWord to retrieve words from the
	// chunkData one by one. Example usage in utils.h
}

// write intermediate data to separate word.txt files
// Each file will have only one line : word 1 1 1 1 1 ...
void writeIntermediateDS() {
	char buffer [200];
	FILE * file;
	
	intermediateDS * tempDS = DSroot;
	valueList * tempVal = NULL;

	if (tempDS == NULL) {
		printf ("ERROR: intermediateDS is empty \n");
		return;
	}

	while (tempDS != NULL){
		sprintf(buffer, "Map_%d/%s.txt", mapperID, tempDS->key);		// create a pathing string Map_<mapperID>/<key>.txt

		char path[] = "./output/MapOut/";								// create an output path to open output Map file
		strcat(path, buffer);											// ./output/MapOut/Map_<mapperID>/<key>.txt

		file = fopen(path, "w");

		if (file == NULL) {
			printf ("Error, cannot open output file");
			return;
		}

		fprintf (file, "%s ", tempDS->key);								// print key first
		tempVal = tempDS -> value;						

		while (tempVal != NULL){										// traverse key's valueList to print 1's
			fprintf (file, "%s ", tempVal->value);	
			tempVal = tempVal -> next;									// increment to next value
		}

		fclose (file);

		tempDS = tempDS -> next;										// increment to next key
	}
}


int main(int argc, char *argv[]) {
	
	if (argc < 2) {
		printf("Less number of arguments.\n");
		printf("./mapper mapperID\n");
		exit(0);
	}

	// ###### DO NOT REMOVE ######
	mapperID = strtol(argv[1], NULL, 10);

    //Print statement for mapper, comment this for final submission
    printf("Mapper id : %d \n",mapperID);

	// ###### DO NOT REMOVE ######
	// create folder specifically for this mapper in output/MapOut
	// mapOutDir has the path to the folder where the outputs of 
	// this mapper should be stored
	mapOutDir = createMapDir(mapperID);
	
	// ###### DO NOT REMOVE ######
	while(1) {
		// create an array of chunkSize=1024B and intialize all 
		// elements with '\0'
		char chunkData[chunkSize + 1]; // +1 for '\0'
		memset(chunkData, '\0', chunkSize + 1);

		char *retChunk = getChunkData(mapperID);
		if(retChunk == NULL) {
			break;
		}

		strcpy(chunkData, retChunk);
		free(retChunk);

		map(chunkData);
	}

	// ###### DO NOT REMOVE ######
	writeIntermediateDS();

    free(mapOutDir);
	return 0;
}