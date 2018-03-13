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
int initializeMap(FILE* fp, char* line, map* array,int lines){
	size_t n = 0;
	int i = 0;

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
	// (**root).listPtr = NULL;
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
/*void printNodes(trieNode *node){
	trieNode* temp = node;

	for(int i = 0; i < temp->childNodes; i++)
		printNodes(temp->children[i]);

	printNode(temp);
}*/

/* Destroys the Trie by freeing the memory
   we allocated, while creating it */
/*void destroyTrie(trieNode* node){
	trieNode* temp = node;
	trieNode* tmp = node;

	while(temp != NULL){
		while(temp->children != NULL)
			temp = temp->children;

		if(temp->next != NULL && temp->children != NULL){
			tmp = temp;
		}
		else if(temp->next != NULL && temp->children == NULL){}
		else if(temp->next == NULL && temp->children != NULL){}
		else if(temp->next == NULL && temp->children == NULL){
			tmp = temp;
			if(temp->letter == 0){
				free(tmp);
				return;
			}
		}
	}*/



	/*trieNode* temp = node;
	if(temp == NULL)
		return;

	for(int i = 0; i < temp->childNodes; i++)
		destroyTrie(temp->children[i]);

	free(temp);
}*/

/* Inserts a new value (word) in the Trie */
int insertTrie(trieNode* node, char* word){
	trieNode* temp = node; 
	trieNode* parent = node;
	trieNode* previous = NULL;
	int flag = -1;
	int code = 0;
	int value; 

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

			if(i < strlen(word)-1)
				temp->children->isEndOfWord = False;
			else
				temp->children->isEndOfWord = True;

			temp->children->children = NULL;
			temp->children->next = NULL;

			if(i < strlen(word)-1)
				temp = temp->children;

			continue;
		}

		else{
			temp = temp->children;

			while(temp != NULL){
				value = compareKeys(&word[i],&(temp->letter));

				if(value > 0){
					previous = temp;
					temp = temp->next;
					continue;
				}

				if(value < 0)
					break;

				if(value == 0){
					flag = i;
					break;
				}
			}

			if(value > 0){
				temp = (trieNode*)malloc(sizeof(trieNode));
				if(temp == NULL)
					return MEMORY_NOT_ALLOCATED;
				temp->next = previous->next;
				previous->next = temp;
				temp->letter = word[i];
				if(i < strlen(word)-1)
					temp->isEndOfWord = False;
				else
					temp->isEndOfWord = True;
				temp->children = NULL;
				previous = NULL;
				continue;
			}

			if(value < 0){
				/* Up until the previous iteration, the letters we
				   were trying to insert were already in the Trie. 
				   This is the first letter that differs and we have 
				   to insert it as the last element of the list */
				if(flag == i-1/*1*/){
					while(temp->next != NULL){
						temp = temp->next;
						if(compareKeys(&word[i],&(temp->letter))==0){
							code = 1;
							break;
						}
					}

					if(code != 1){
						temp->next = (trieNode*)malloc(sizeof(trieNode));
						if(temp->next == NULL)
							return MEMORY_NOT_ALLOCATED;
							
						temp->next->letter = word[i];

						if(i < strlen(word)-1)
							temp->next->isEndOfWord = False;
						else
							temp->next->isEndOfWord = True;

						temp->next->children = NULL;
						temp->next->next = NULL;
						temp = temp->next;
						previous = NULL;
						continue;
					}
				}

				/* Insert at the beginning */
				else if(previous == NULL){
					previous = (trieNode*)malloc(sizeof(trieNode));
					if(previous == NULL)
						return MEMORY_NOT_ALLOCATED;
					previous->letter = word[i];

					if(i < strlen(word)-1)
						previous->isEndOfWord = False;
					else
						previous->isEndOfWord = True;

					previous->children = NULL;
					previous->next = temp;
					parent->children = previous;
					temp = previous;
					previous = NULL;

					continue;
				}
				else{
					temp = (trieNode*)malloc(sizeof(trieNode));
					if(temp == NULL)
						return MEMORY_NOT_ALLOCATED;
					temp->letter = word[i];

					if(i < strlen(word)-1)
						temp->isEndOfWord = False;
					else
						temp->isEndOfWord = True;

					temp->children = NULL;
					temp->next = previous->next;
					previous->next = temp;
					previous = NULL;
					continue;
				}
			}

			if(code == 1 && i == strlen(word)-1)
				temp->isEndOfWord = True;
			
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
			code = insertTrie(node,token);
			if(code != OK)
				return code;
			token = strtok(NULL," \t");
		}
	}
	return OK;
}


/**********************/
/*** FILE FUNCTIONS ***/
/**********************/

/* Returns the total number of lines of a file */
int getNumberOfLines(FILE* fp, char* lineptr){
	size_t n = 0;
	int lines = 0;
	while(getline(&lineptr,&n,fp)!=-1){
		lines++;
	}
	return lines;
}


