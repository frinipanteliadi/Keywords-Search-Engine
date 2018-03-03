#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
	int id;
	char *text;
}node;

int main(int argc, char *argv[]){

	/*********************/
	/*** OPEN THE FILE ***/
	/*********************/
	FILE *fp;
	fp = fopen(argv[2],"r");
	if(fp == NULL){
		printf("An error occured while trying to");
		printf(" open the file\n");
		return -1;
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
			return -1;
		}
		lines++;
	}

	fclose(fp);
	free(lineptr);
	
	/****************************/
	/***    SAVE THE TEXTS    ***/
	/***  ALONG WITH THEIR ID ***/
	/***************************/	

	node *array;
	array = (node*)malloc(lines*sizeof(node));
	if(array == NULL){
		printf("An error occured while trying to");
		printf(" allocate memory\n");
		return -1;
	}

	fp = fopen(argv[2],"r");
	if(fp == NULL){
		printf("An error occured while trying to");
		printf(" open the file\n");
		return -1;
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