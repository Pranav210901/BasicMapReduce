#include "reducer.h"
finalKeyValueDS *node;
// create a key value node
finalKeyValueDS *createFinalKeyValueNode(char *word, int count){
	finalKeyValueDS *newNode = (finalKeyValueDS *)malloc (sizeof(finalKeyValueDS));
	strcpy(newNode -> key, word);
	newNode -> value = count;
	newNode -> next = NULL;
	return newNode;
}

// insert or update an key value
finalKeyValueDS *insertNewKeyValue(finalKeyValueDS *root, char *word, int count){
	finalKeyValueDS *tempNode = root;
	if(root == NULL)
		return createFinalKeyValueNode(word, count);
	while(tempNode -> next != NULL){
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value += count;
			return root;
		}
		tempNode = tempNode -> next;
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value += count;
	} else{
		tempNode -> next = createFinalKeyValueNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeFinalDS(finalKeyValueDS *root) {
	if(root == NULL) return;
	finalKeyValueDS *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		free(tempNode);
	}
}

// reduce function
void reduce(char *key) 
{
	//initializing variables
	char array[50];		//will hold the first word
	char valarray[50];	//will be used to iterate through file in while loop
	int count = 0;		//counter for 1's 
	
	FILE *fp;
	fp = fopen(key, "r");
	
	if(fp == NULL)
	{
		printf("file not found");
		return;
	}
	//extracting the first word only to pass into insertNewKeyValue
	fscanf(fp, "%s ", array);	
	//while loop to extract the count of 1's
	while (fscanf(fp, "%s ", valarray) != EOF)
	{
	count++;		//counting 1's
	}
	node = insertNewKeyValue(node, array, count);
	fclose(fp);
    
	
}

// write the contents of the final intermediate structure
// to output/ReduceOut/Reduce_reducerID.txt
void writeFinalDS(int reducerID){
	finalKeyValueDS *temp = node;	//Data Structure

	//appending the entire path to a concatenated string
	char buffer[200];							//holds specific path (filename)
	char concstr[] = "./output/ReduceOut/";		//holds general path fo directory (folders)
	sprintf(buffer, "Reduce_%d.txt", reducerID);
	//string 1 = ./output... string 2 = buffer
	// concatenate
	strcat(concstr, buffer);

	FILE *fpw;		
	fpw = fopen(concstr, "w");					//opening Reduce_reducerID to write

	//FILE not found error
	if (fpw == NULL){
        printf("ERROR: File DNE /n");
        return;
    }  

	//Empty Data Structure Error
	if (temp == NULL) {
        printf ("ERROR: intermediateDS is empty \n");
        return;
    }

	//iterating through DS and writing to file
	while (temp != NULL){
		fprintf (fpw, "%s ", temp->key);					// print key first
		fprintf(fpw,"%d \n",temp->value);					//print value then
		temp = temp->next;									//move to the next <key,value>								
	}
	fclose(fpw);
}

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Less number of arguments.\n");
		printf("./reducer reducerID");
	}
	// ###### DO NOT REMOVE ######
	// initialize 
	int reducerID = strtol(argv[1], NULL, 10);
    //Print statement for reducer, comment this for final submission
    //printf("Reducer id : %d \n",reducerID);
	// ###### DO NOT REMOVE ######
	// master will continuously send the word.txt files
	// alloted to the reducer
	char key[MAXKEYSZ];
	while(getInterData(key, reducerID))
		reduce(key);
	// You may write this logic. You can somehow store the
	// <key, value> count and write to Reduce_reducerID.txt file
	// So you may delete this function and add your logic
	writeFinalDS(reducerID);
	return 0;
}
