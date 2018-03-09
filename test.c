#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#define OK 0
#define FILE_NOTOPEN -1
#define MEMORY_ALLOCATIONERROR -2
#define TEXTS_WRONG -3
#define ERROR -4


/* Command for terminal: ./minisearch -i docfile -k K */

int main(int argc, char *argv[]){

	if(argc != 5){
		printf("Error! Not enough arguments");
		printf(" were provided\n");
		return -1;
	}

	/*********************/
	/*** OPEN THE FILE ***/
	/*********************/
	FILE *fp;
	fp = fopen(argv[2],"r");
	if(fp == NULL){
		printf("An error occured while trying to");
		printf(" open the file\n");
		return FILE_NOTOPEN;
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
		printf("An error occured while trying to");
		printf(" allocate memory for the map\n");
		return MEMORY_ALLOCATIONERROR;
	}

	char *line = NULL;
	int code = initializeMap(fp,line," ",array);
	if(code == -1)
		return -1;
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
	root = (trieNode*)malloc(sizeof(trieNode*));
	if(root == NULL)
		return MEMORY_ALLOCATIONERROR;

	initializeRoot(&root);
	// printNodes(root);		
	
	initializeTrie(lines," ",root,array);

	// printNodes(root);
	/***************************/
	/*** DEALLOCATING MEMORY ***/
	/***************************/
	
	for(int i = 0; i<lines; i++)
		free(array[i].text);
	free(array);
	destroyTrie(root);
	return 0;
}