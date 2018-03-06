#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define FILE_NOTOPEN -1
#define MEMORY_ALLOCATIONERROR -2
#define TEXTS_WRONG -3
#define ERROR -4

/* False = 0, True = 1 */
typedef enum bool {False, True} bool;

typedef struct map{
	int id;
	char *text;
}map;

typedef struct postingsList{
	int test;
}postingsList;

typedef struct trieNode{
	char letter;					/*Letter stored inside the node*/
	bool isEndOfWord;				/*Flag which indicated whether a word ends in the current node*/
	struct trieNode **children;		/*An array of pointers, where each one will store the address of a child node*/
	postingsList *listPtr;			/*Pointer to postings list of a leaf node*/
	int arraySize;					/*Size of the array of pointers (needed for use with realloc)*/
}trieNode;


nt main(int argc, char *argv[]){

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

	/********************************/
	/*** FIND THE TOTAL NUMBER OF ***/
	/***    TEXTS IN THE FILE     ***/
	/********************************/
	char *lineptr;
	lineptr = NULL;
	size_t n = 0;
	int lines = 0;
	while(getline(&lineptr, &n,fp)!=-1){
		char *id;
		id = strtok(lineptr," ");
		//printf("ID: %s\n",id);
		int id1 = atoi(id);
		if(id1 != lines){
			printf("The text were not given in the");
			printf(" right order!\n");
			return TEXTS_WRONG;
		}
		lines++;
	}

	fclose(fp);
	free(lineptr);
	
	/****************************/
	/***    SAVE THE TEXTS    ***/
	/***  ALONG WITH THEIR ID ***/
	/***************************/	

	map *array;
	array = (map*)malloc(lines*sizeof(map));
	if(array == NULL){
		printf("An error occured while trying to");
		printf(" allocate memory\n");
		return MEMORY_ALLOCATIONERROR;
	}

	fp = fopen(argv[2],"r");
	if(fp == NULL){
		printf("An error occured while trying to");
		printf(" open the file\n");
		return FILE_NOTOPEN;
	}

	char *line;
	line = NULL;
	n = 0;
	int i = 0;
	while(getline(&line,&n,fp)!=-1){
		char *id;
		id = strtok(line," ");
		array[i].id = atoi(id);
		char *text = strtok(NULL,"\n");
		array[i].text = (char*)malloc(strlen(text)+1);
		strcpy(array[i].text,text);
		//printf("TEXT: %s\n",text);
		//printf("TEXT IN ARRAY: %s\n",array[i].text);
		i++;
	}

	for(int i = 0; i < lines; i++){
		printf("ID: %d\n",array[i].id);
		printf("TEXT: %s\n",array[i].text);
		printf("\n");
	}

	for(int i = 0; i<lines; i++)
		free(array[i].text);

	free(line);
	fclose(fp);
	free(array);
	return 0;
}