#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

/* Command for terminal: ./minisearch -i docfile -k K */

int main(int argc, char *argv[]){

	if(argc != 5){
		printError(NOT_ENOUGH_ARGUMENTS);
		return EXIT;
	}

	/*********************/
	/*** OPEN THE FILE ***/
	/*********************/
	FILE *fp;
	fp = fopen(argv[2],"r");
	if(fp == NULL){
		printError(FILE_NOT_OPEN);
		return EXIT;
	}

	/***********************************/
	/*** FINDING THE TOTAL NUMBER OF ***/
	/***      TEXTS IN THE FILE      ***/
	/***********************************/
	char* lineptr = NULL;
	int lines = getNumberOfLines(fp);
	// printf("The file has %d texts in total\n",lines);
	rewind(fp);
	free(lineptr);
	

	/************************/	
	/*** CREATING THE MAP ***/
	/************************/
	
	map *array;
	array = (map*)malloc((lines)*sizeof(map));
	if(array == NULL){
		printError(MEMORY_NOT_ALLOCATED);
		return EXIT;
	}


	int totalWords = 0;
	int code = initializeMap(fp,array,lines,&totalWords);
	if(code != OK){
		printError(code);
		return EXIT;
	}

	double average = averageNumberOfWords(totalWords,lines);
	// printf("Total words in the document: %d\n",totalWords);
	// printf("Average number of words: %1.2f\n",average);
	// printMap(lines,array); 

	/************************/
	/*** CLOSING THE FILE ***/
	/************************/
	fclose(fp);

	/*********************************/
	/*** CREATING AND INITIALIZING ***/
	/***   THE ROOT OF THE TRIE    ***/
	/*********************************/
	
	trieNode* root;
	root = (trieNode*)malloc(sizeof(trieNode));
	if(root == NULL){
		printError(MEMORY_NOT_ALLOCATED);
		return EXIT;
	}

	initializeRoot(&root);
	// printNodes(root);		

	/*************************/
	/*** CREATING THE TRIE ***/ 
	/***    STRUCTURE      ***/
	/*************************/

	code = initializeTrie(lines,root,array);
	if(code != OK){
		printError(code);
		return EXIT;
	}

	// printMap(lines,array);
	// printNodes(root);


	/**********************/
	/*** USER INTERFACE ***/
	/**********************/

	welcomeMessage();

	char *input = NULL;
	size_t n = 0;
	while(getline(&input,&n,stdin)!=-1){
		input = strtok(input,"\n");					/* Input provided by the user */
		printf("You entered: %s\n",input);
		
		char* operation = strtok(input," \t");		/* Operation required by the user */ 
		char* arguments = strtok(NULL,"\n");		/* The arguments of the command */
		if(arguments != NULL)
			printf("Arguments: %s\n",arguments);
	
		if(strcmp(operation,"/search") == 0){
			printf("You requested a search\n");
			code = searchOperation(root,arguments,lines,array,average,2);
			if(code != OK){
				printError(code);
				break;
			}
		}
		else if(strcmp(operation,"/df") == 0)
			printf("You requested the document frequency vector\n");
		else if(strcmp(operation,"/tf") == 0)
			printf("You requested the term frequency of a word\n");
		else if(strcmp(operation,"/exit") == 0){
			printf("Exiting the application\n");
			break;
		}
		else
			printf("Invalid input. Try again\n");
	}

	printf("\n");
	free(input);

	// postingsList* ptr;
	// ptr = searchTrie(root,search);
	// printPostingsList(ptr,search);

	/***************************/
	/*** DEALLOCATING MEMORY ***/
	/***************************/
	
	for(int i = 0; i<lines; i++)
		free(array[i].text);
	free(array);
	destroyTrie(root);
	// free(command);
	return OK;
}