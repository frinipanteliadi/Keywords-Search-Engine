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

	/*array[0].id = 0;
	char *text = "My name is Frini and I am 21";
	array[0].text = (char*)malloc(strlen(text)+1);
	if(array[0].text == NULL)
		return MEMORY_NOT_ALLOCATED;
	strcpy(array[0].text,text);*/
	// printf("%s\n",array[0].text);

	int code = initializeMap(fp,array,lines);
	if(code != OK){
		printError(code);
		return EXIT;
	}

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

	// printNodes(root);
	printf("*******************\n\n");
	printf("Enter the word you would like to search\n");
	char* word;
	scanf("%s",word);
	searchTrie(root,word);
	printf("\n\n*******************\n");
	
	/***************************/
	/*** DEALLOCATING MEMORY ***/
	/***************************/
	
	for(int i = 0; i<lines; i++)
		free(array[i].text);
	free(array);
	destroyTrie(root);
	return OK;
}