#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

//     Commands for terminal: 
// 	./minisearch -i docfile -k K 
// 	./minisearch -k K -i docfile
// 	./minisearch -i docfile

int main(int argc, char *argv[]){

	/******************************/
	/***   OPENING THE FILE     ***/
	/***	     AND            ***/
	/*** HANDLING THE ARGUMENTS ***/
	/******************************/
	if(argc < 3){
		printError(ARGUMENTS_ERROR);
		return EXIT;
	}
	else if(argc == 4){
		printError(ARGUMENTS_ERROR);
		return EXIT;
	}
	else if(argc > 5){
		printError(ARGUMENTS_ERROR);
		return EXIT;
	}


	FILE *fp;
	char* docfile;
	int k = 10;

	for(int i = 1; i < argc; i+=2){
		if(strcmp(argv[i],"-i") == 0){
			fp = fopen(argv[i+1],"r");
			if(fp == NULL){
				printError(FILE_NOT_OPEN);
				return ERROR;
			}
		}
		else if(strcmp(argv[i],"-k") == 0)
			k = atoi(argv[i+1]);
	}

	/***********************************/
	/*** FINDING THE TOTAL NUMBER OF ***/
	/***      TEXTS IN THE FILE      ***/
	/***********************************/
	char* lineptr = NULL;
	int lines = getNumberOfLines(fp);
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

	/*************************/
	/*** CREATING THE TRIE ***/ 
	/***    STRUCTURE      ***/
	/*************************/

	code = initializeTrie(lines,root,array);
	if(code != OK){
		printError(code);
		return EXIT;
	}

	/**********************/
	/*** USER INTERFACE ***/
	/**********************/

	welcomeMessage();

	char *input = NULL;
	size_t n = 0;
	while(getline(&input,&n,stdin)!=-1){
		input = strtok(input,"\n");					/* Input provided by the user */
		
		char* operation = strtok(input," \t");		/* Operation required by the user */ 
		char* arguments = strtok(NULL,"\n");		/* The arguments of the command */
		
		if(strcmp(operation,"/search") == 0){
			code = searchOperation(root,arguments,lines,array,average,k);
			if(code != OK){
				printError(code);
				break;
			}
		}
		else if(strcmp(operation,"/df") == 0){
			code = dfOperation(root,arguments,lines,array);
			if(code != OK){
				printError(code);
				break;
			}
		}
		else if(strcmp(operation,"/tf") == 0){
			code = tfOperation(root,arguments);
			if(code != OK){
				printError(code);
				break;
			}
		}
		else if(strcmp(operation,"/exit") == 0){
			printf("Exiting the application\n");
			break;
		}
		else
			printf("Invalid input. Try again\n");
		printf("\n\n");
	}

	printf("\n");
	free(input);

	/***************************/
	/*** DEALLOCATING MEMORY ***/
	/***************************/
	
	for(int i = 0; i<lines; i++)
		free(array[i].text);
	free(array);
	destroyTrie(root);
	return OK;
}