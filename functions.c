#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

/***************************/
/*** ASSISTIVE FUNCTIONS ***/
/***************************/

/* Returns the difference between the ASCII values
   of a and b */
int compareKeys(char* a, char* b){
	return((*a)-(*b));
}	


/**********************/
/*** ERROR HANDLING ***/
/**********************/

void printError(int error){
	switch(error){
		case NOT_ENOUGH_ARGUMENTS:
			printf("Not enough arguments were provided");
			printf(" in the command line\n");
			break;
		case FILE_NOT_OPEN:
			printf("Failure to open the file\n");
			break;
		case MEMORY_NOT_ALLOCATED:
			printf("An error occured while trying to");
			printf(" allocate memory\n");
			break;
		case WRONG_ORDER:
			printf("The provided texts were given in");
			printf(" the wrong order\n");
			break;
	}
}


/*********************/
/*** MAP FUNCTIONS ***/
/*********************/

/* Parses the texts from the file into the map */
int initializeMap(FILE* fp, map* array,int lines){
	size_t n = 0;
	int i = 0;
	char *line = NULL;
	while(getline(&line,&n,fp)!=-1){
		if(i > lines)
			return ERROR;
		// printf("Line: %s\n",line);
		char *id;
		id = strtok(line," \t");
		if(i != atoi(id))
			return WRONG_ORDER;
		
		array[i].id = atoi(id);
		char *text = strtok(NULL,"\n");
		// printf("text: %s\n",text);
		array[i].text = (char*)malloc(strlen(text)+1);
		if(array[i].text == NULL)
			return MEMORY_NOT_ALLOCATED;
		strcpy(array[i].text,text);
		// printf("array[%d].text: %s\n",i,array[i].text);
		i++;
	}
	free(line);
	return OK;
}

void printMap(int lines, map* ptr){
	map* temp = ptr;
	for(int i = 0; i < lines; i++){
		printf("-----------------------\n");
		printf(" *ID: %d\n",temp[i].id);
		printf(" *TEXT: %s\n",temp[i].text);
		printf("-----------------------\n");
	}
}


/***************************/
/***    TRIE FUNCTIONS   ***/
/***************************/

/* Initializes all of the root's members */
void initializeRoot(trieNode **root){

	(**root).letter = 0;
	(**root).isEndOfWord = False;
	(**root).listPtr = NULL;
	(**root).children = NULL;
	(**root).next = NULL;
}

/* Prints the values of a node's members */
void printNode(trieNode *node){
	trieNode* temp = node;
	printf("------------------\n");
	printf(" *Letter: %c\n",temp->letter);
	printf(" *isEndOfWord: %d\n",temp->isEndOfWord);
	printf(" *children: %x\n",temp->children);
	if(temp->children != NULL)
		printf(" *childValue: %c\n",temp->children->letter);
	printf(" *next: %x\n",temp->next);
	if(temp->next != NULL)
		printf(" *nextValue: %c\n",temp->next->letter);
	printf("------------------\n");
}

/* Prints the data that each member of the 
   Trie holds */
void printNodes(trieNode *node){
	if(node == NULL)
		return;

	printNodes(node->next);
	printNodes(node->children);
	printNode(node);
}

/* Inserts a new value (word) in the Trie */
int insertTrie(trieNode* node, char* word,int textID){
	trieNode* temp = node; 
	trieNode* parent = node;
	trieNode* previous = NULL;
	int value; 
	int errorCode;

	// printf("Inserting the word: %s\n",word);

	/* Each iteration is responsible for a single
	   letter of the word we're trying to insert */
	for(int i = 0; i < strlen(word); i++){
		// printf("Inserting the letter %c\n",word[i]);

		if(temp->children == NULL){
			temp->children = (trieNode*)malloc(sizeof(trieNode));
			if(temp->children == NULL)
				return MEMORY_NOT_ALLOCATED;

			temp->children->letter = word[i];
			temp->children->isEndOfWord = False;
			temp->children->children = NULL;
			temp->children->next = NULL;
			temp->children->listPtr = NULL;

			temp = temp->children;

			if(i == strlen(word)-1) {
				temp->isEndOfWord = True;
				errorCode = addList(&(temp->listPtr), textID);
				if(errorCode != OK)
					return errorCode;
				// printPostingsList(temp->listPtr,word);
			}
		}

		else{
			parent = temp;
			temp = temp->children;

			while(temp != NULL){
				value = compareKeys(&word[i],&(temp->letter));

				if(value > 0){
					previous = temp;
					temp = temp->next;
					continue;
				}

				if(value <= 0)
					break;
			}

			if(value > 0){
				temp = (trieNode*)malloc(sizeof(trieNode));
				if(temp == NULL)
					return MEMORY_NOT_ALLOCATED;
				temp->next = previous->next;
				previous->next = temp;
				temp->letter = word[i];
				temp->isEndOfWord = False;
				temp->children = NULL;
				temp->listPtr = NULL;
			}

			if(value < 0){
				/* Up until the previous iteration, the letters we
				   were trying to insert were already in the Trie. 
				   This is the first letter that differs and we have 
				   to insert it as the last element of the list */

				/* Insert at the beginning */
				if(previous == NULL){
					previous = (trieNode*)malloc(sizeof(trieNode));
					if(previous == NULL)
						return MEMORY_NOT_ALLOCATED;
					previous->letter = word[i];

					previous->isEndOfWord = False;
					previous->children = NULL;
					previous->next = temp;
					previous->listPtr = NULL;
					parent->children = previous;
					temp = previous;
				}
				else{
					temp = (trieNode*)malloc(sizeof(trieNode));
					if(temp == NULL)
						return MEMORY_NOT_ALLOCATED;

					temp->letter = word[i];
					temp->isEndOfWord = False;
					temp->listPtr = NULL;
					temp->children = NULL;
					temp->next = previous->next;
					previous->next = temp;
				}
			}

			if(i == strlen(word)-1) {
				temp->isEndOfWord = True;
				errorCode = addList(&(temp->listPtr), textID);
				if(errorCode != OK)
					return errorCode;
				// printPostingsList(temp->listPtr,word);
			}
			
			previous = NULL;	
		}
	}

	return OK;
}

/* Initializes the Trie by inserting its first values.
   Basically, it's responsible for the creation of 
   the Trie */
int initializeTrie(int lines, trieNode* node, map* array){
	char* token;
	int code;
	// int textID;
	for(int i = 0; i < lines; i++){
		// textID = array[i].id;
		token = strtok(array[i].text," \t");
		while(token!=NULL){
			code = insertTrie(node,token,array[i].id);
			if(code != OK)
				return code;
			token = strtok(NULL," \t");
		}
	}
	return OK;
}

/* Destroys the Trie by freeing the memory
   we allocated, while creating it */
void destroyTrie(trieNode* node){
	if(node == NULL)
		return;

	destroyTrie(node->next);
	destroyTrie(node->children);
	deletePostingsList(node->listPtr);
	free(node);
}

void searchTrie(trieNode* node,char* word){
	trieNode* temp = node;
	trieNode* previous = NULL;
	int value;
	for(int i = 0; i < strlen(word); i++){
		
		temp = temp->children;
		while(temp != NULL){
			value = compareKeys(&word[i],&(temp->letter));

			if(value > 0){
				previous = temp;
				temp = temp->next;
				continue;
			}

			if(value <= 0)
				break;
		}

		if(value > 0){
			printf("1)The word '%s' is not in the Trie\n",word);
			return;
		}

		if(value < 0){
			printf("2)The word '%s' is not in the Trie\n",word);
			return;
		}

		if(i == strlen(word)-1){
			if(temp->isEndOfWord == True){
				printf("The word '%s' is in the Trie\n",word);
				return;
			}
		}
		previous = NULL;
		
	}
}

/**********************/
/*** FILE FUNCTIONS ***/
/**********************/

/* Returns the total number of lines of a file */
int getNumberOfLines(FILE* fp){
	size_t n = 0;
	int lines = 0;
	char* lineptr = NULL;
	while(getline(&lineptr,&n,fp)!=-1){
		lines++;
	}
	free(lineptr);
	return lines;
}


/*********************/
/*** POSTINGS LIST ***/ 
/***   FUNCTIONS   ***/
/*********************/

int addList(postingsList** ptr, int textID){

	postingsList* parent;
	postingsListNode* temp;
	postingsListNode* previous;
	int value;

	if(*ptr == NULL){
		*ptr = (postingsList*)malloc(sizeof(postingsList));
		if(*ptr == NULL)
			return MEMORY_NOT_ALLOCATED;
		(*ptr)->dfVector = 1;
		(*ptr)->headPtr = (postingsListNode*)malloc(sizeof(postingsListNode));
		if((*ptr)->headPtr == NULL)
			return MEMORY_NOT_ALLOCATED;

		(*ptr)->headPtr->textID = textID;
		(*ptr)->headPtr->occurences = 1;
		(*ptr)->headPtr->next = NULL;
	}
	
	else{
		parent = *ptr;
		temp = parent->headPtr;

		while(temp != NULL){
			value = (textID - temp->textID);

			if(value == 0)
				break;
			if(value > 0){
				previous = temp;
				temp = temp->next;
				continue;
			}
		}

		if(value == 0)
			temp->occurences++;
		else if(value > 0){
			temp = (postingsListNode*)malloc(sizeof(postingsListNode));
			if(temp == NULL)
				return MEMORY_NOT_ALLOCATED;
			temp->textID = textID;
			temp->occurences = 1;
			temp->next = previous->next;
			previous->next = temp;
			parent->dfVector++;
		}
	}

	return OK;
}

void deleteList(postingsListNode* ptr){
	if(ptr == NULL)
		return;
	deleteList(ptr->next);
	free(ptr);
}

void deletePostingsList(postingsList* ptr){
	if(ptr == NULL)
		return;

	postingsListNode* temp = ptr->headPtr;
	deleteList(temp);
}

void printPostingsList(postingsList* ptr, char* word){
	postingsListNode* temp = ptr->headPtr;
	printf("\n******************\n");
	printf("WORD: %s\n",word);
	printf("dfVector: %d\n",ptr->dfVector);
	while(temp != NULL){
		printf("------------------\n");
		printf("textID: %d\n",temp->textID);
		printf("Occurences: %d\n",temp->occurences);
		printf("Next: %x\n",temp->next);
		printf("------------------\n");
		temp = temp->next;
	}
	printf("******************\n\n");
}