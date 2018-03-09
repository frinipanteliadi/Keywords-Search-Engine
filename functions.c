#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

/*********************/
/*** MAP FUNCTIONS ***/
/*********************/

/* Parses the texts from the file into the map */
int initializeMap(FILE* fp, char* line, char* delimiter,map* array){
	size_t n = 0;
	int i = 0;

	while(getline(&line,&n,fp)!=-1){
		char *id;
		id = strtok(line," ");
		if(i != atoi(id)){
			printf("Texts were given in the wrong order\n");
			return -1;
		}
		array[i].id = atoi(id);
		char *text = strtok(NULL,"\n");
		array[i].text = (char*)malloc(strlen(text)+1);
		if(array[i].text == NULL)
			return -1;
		strcpy(array[i].text,text);
		i++;
	}


	/*char str[1000];
	while(fgets(str,1000,fp)!=NULL)
		printf("%s\n",str);*/

	return 0;
}

void printMap(int lines, map* ptr){
	for(int i = 0; i < lines; i++){
		printf("-----------------------\n");
		printf(" *ID: %d\n",ptr[i].id);
		printf(" *TEXT: %s\n",ptr[i].text);
		printf("-----------------------\n");
	}
}

/***************************/
/*** TRIE ROOT FUNCTIONS ***/
/***************************/
/* Returns the difference between the ASCII values
   of a and b */
int compareKeys(char* a, char* b){
	return((*a)-(*b));
}	

/* Initializes all of the root's members */
void initializeRoot(trieNode **root){

	(**root).letter = 0;
	(**root).isEndOfWord = False;
	// (**root).listPtr = NULL;
	(**root).arraySize = 0;
	(**root).childNodes = 0;
	(**root).children = NULL;
}

/* Prints the values of a node's members */
void printNode(trieNode *node){
	trieNode* temp = node;
	printf("------------------\n");
	printf(" *Letter: %c\n",temp->letter);
	printf(" *isEndOfWord: %d\n",temp->isEndOfWord);
	printf(" *arraySize: %d\n",temp->arraySize);
	printf(" *childNodes: %d\n",temp->childNodes);
	printf(" *children: %x\n",temp->children);
	printf("------------------\n");
}

/* Prints the data that each member of the 
   Trie holds */
void printNodes(trieNode *node){
	trieNode* temp = node;

	for(int i = 0; i < temp->childNodes; i++)
		printNodes(temp->children[i]);

	printNode(temp);
}

/* Destroys the Trie by freeing the memory
   we allocated, while creating it */
void destroyTrie(trieNode* node){
	trieNode* temp = node;

	if(temp == NULL)
		return;

	for(int i = 0; i < temp->childNodes; i++)
		destroyTrie(temp->children[i]);

	free(temp);
}

/* Inserts a new value (word) in the Trie */
void insertTrie(trieNode* node, char* word){
	trieNode* temp = node;

	/* Each iteration is responsible for a single
	   letter of the word we're trying to insert */
	for(int i = 0; i < strlen(word); i++){

		/* The node we're currently working on doesn't have
		   any children */
		if(temp->children == NULL){
			temp->arraySize = 2;
			temp->childNodes++;

			temp->children = (trieNode**)malloc((temp->arraySize)*sizeof(trieNode*));
			if(temp->children == NULL)
				return;

			for(int j=0; j<(temp->arraySize); j++){
				if(j>0)
					temp->children[j] = NULL;
				else{
					temp->children[0] = (trieNode*)malloc(sizeof(trieNode));
					if(temp->children[0] == NULL)
						return;

					temp->children[0]->letter = word[i];

					/* Checking whether the current letter is the final one
					   of a word */
					if(i < (strlen(word)-1)){
						temp->children[0]->isEndOfWord = False;
						// temp->children[0]->listPtr = NULL;
					}
					else{
						temp->children[0]->isEndOfWord = True;
						// temp->children[0]->listPtr points to the head of a postings list
					}

					temp->children[0]->arraySize = 0;
					temp->children[0]->childNodes = 0;
					temp->children[0]->children = NULL;
				}
			}
			
			/* If there are still some letters left, we have
			   to make the pointer point at the node we 
			   recently created */
			if(i < strlen(word)-1)
				temp = temp->children[0];
		}
		
		/* The node we're currently on has at least
		   one child */
		else{
			
			/* PROBLEM: Not necessary if we're increasing an
			   already stored word */
			if(temp->childNodes == temp->arraySize){
				temp->arraySize = (temp->arraySize)*2;
				temp->children = (trieNode**)realloc(temp->children,(temp->arraySize)*sizeof(trieNode*));
				if(temp->children == NULL)
					return;
				for(int k = temp->childNodes; k < temp->arraySize; k++)
					temp->children[k] = NULL;
			}

			int value;
			int j;
			for(j = 0; j < temp->childNodes; j++){
				value = compareKeys(&word[i],&(temp->children[j]->letter));

				if(value > 0)
					continue;
				if(value == 0)
					break;
				if(value < 0){
					// memmove(temp->children[j+1],temp->children[j],(size_t)((temp->childNodes-j)*sizeof(trieNode*)));
					for(int k = temp->arraySize - 1; k > j; k--)
						temp->children[k] = temp->children[k-1];
					temp->children[j] = NULL;
					break;
				}
			} 

			if(temp->children[j] == NULL){
				temp->childNodes++;	

				temp->children[j] = (trieNode*)malloc(sizeof(trieNode));
				if(temp->children[j] == NULL)
					return;

				temp->children[j]->letter = word[i];

				/* The letter we just inserted does not
				   indicate the end of a word */
				if(i < (strlen(word)-1)){
					temp->children[j]->isEndOfWord = False;
					// temp->children[j]->listPtr = NULL;
				}
				else{
					temp->children[j]->isEndOfWord = True;
					// temp->children[0]->listPtr points to the head of a postings list
				}

				/* The current node doesn't have any children.
				   That's why we must initialize the following 
				   values with zero */
				temp->children[j]->arraySize = 0;
				temp->children[j]->childNodes = 0;
				temp->children[j]->children = NULL;
			}

			/* If we still have letters that we need to insert,
			   we must change the node that we're working on */
			if(i < strlen(word)-1)
				temp = temp->children[j];
		}
	}

	return;
}

/* Initializes the Trie by inserting its first values */
void initializeTrie(int lines, char *delimiter, trieNode* node, map* array){
	char* token;
	for(int i = 0; i < lines; i++){
		token = strtok(array[i].text,delimiter);
		while(token!=NULL){
			insertTrie(node,token);
			token = strtok(NULL,delimiter);
		}
	}
}


/**********************/
/*** FILE FUNCTIONS ***/
/**********************/
int getNumberOfLines(FILE* fp, char* lineptr){
	size_t n = 0;
	int lines = 0;
	while(getline(&lineptr,&n,fp)!=-1){
		lines++;
	}
	return lines;
}
