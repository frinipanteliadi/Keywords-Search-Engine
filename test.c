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
	int lines = getNumberOfLines(fp,lineptr);
	// printf("The file has %d texts in total\n",lines);
	rewind(fp);
	free(lineptr);
	

	/**********************/	
	/*** CREATING THE MAP ***/
	/**********************/
	
	map *array;
	array = (map*)malloc(lines*sizeof(map));
	if(array == NULL){
		printError(MEMORY_NOT_ALLOCATED);
		return EXIT;
	}

	char *line = NULL;
	int code = initializeMap(fp,line," ",array);
	if(code != OK){
		printError(code);
		return EXIT;
	}


	/**********************/
	/*** CLOSING THE FILE ***/
	/**********************/
	free(line);
	fclose(fp);

	
	// printMap(lines,array); 

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
	
	code = initializeTrie(lines," ",root,array);
	if(code != OK){
		printError(code);
		return EXIT;
	}
	
	// printNodes(root);

	
	/***************************/
	/*** DEALLOCATING MEMORY ***/
	/***************************/
	
	for(int i = 0; i<lines; i++)
		free(array[i].text);
	free(array);
	// destroyTrie(root);
	return 0;
}