#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "functions.h"

/***************************/
/*** ASSISTIVE FUNCTIONS ***/
/***************************/

/* Returns the difference between the ASCII values
   of a and b */
int compareKeys(char* a, char* b){
	return((*a)-(*b));
}	

/* Returns the total number of words in a string */
int getNumberOfWords(char* str){
	int i = 0;
	int words = 1;

	while(str[i] != '\0'){
		if(str[i]==' ' || str[i]=='\n' || str[i]=='\t')
			words++;
		i++;
	}

	return words;
}

double averageNumberOfWords(int a, int b){
	return((double)a)/((double)b);
}

void welcomeMessage(void){
	printf("\nWelcome! Please choose one of the following options:\n");
	printf("\n* /search: To search for documents containing the word(s)\n");
	printf("* /df: To print the document frequency vector\n");
	printf("* /tf: To print the term frequency of a word\n");
	printf("* /exit: To exit the application\n\n");
}


/**********************/
/*** ERROR HANDLING ***/
/**********************/

void printError(int error){
	switch(error){
		case ARGUMENTS_ERROR:
			printf("The provided arguments are not");
			printf(" in the right form\n");
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
		case ERROR:
			printf("An error occured\n");
			break;
	}
}


/*********************/
/*** MAP FUNCTIONS ***/
/*********************/

/* Passes the texts from the file into the map */
int initializeMap(FILE* fp, map* array,int lines, int* totalWords){
	size_t n = 0;
	int i = 0;
	char *line = NULL;
	int total = 0;
	while(getline(&line,&n,fp)!=-1){
		if(i == lines)
			break;
		if(strlen(line) == 1)
			continue;
		char *id;
		id = strtok(line," \t");
		if(i != atoi(id))
			return WRONG_ORDER;
		
		array[i].id = atoi(id);
		char *text = strtok(NULL,"\n");
		array[i].text = (char*)malloc(strlen(text)+1);
		if(array[i].text == NULL)
			return MEMORY_NOT_ALLOCATED;
		strcpy(array[i].text,text);
		array[i].words = getNumberOfWords(array[i].text);
		total += array[i].words;
		i++;
	}

	*totalWords = total;
	free(line);
	return OK;
}

void printMap(int lines, map* ptr){
	map* temp = ptr;
	for(int i = 0; i < lines; i++){
		printf("-----------------------\n");
		printf(" *ID: %d\n",temp[i].id);
		printf(" *TEXT: %s\n",temp[i].text);
		printf(" *WORDS: %d\n",temp[i].words);
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
				errorCode = addList(&(temp->listPtr), textID, word);
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
				errorCode = addList(&(temp->listPtr), textID, word);
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
	for(int i = 0; i < lines; i++){
		char* textCopy = (char*)malloc(strlen(array[i].text)+1);
		strcpy(textCopy,array[i].text);
		if(textCopy == NULL)
			return MEMORY_NOT_ALLOCATED;
		token = strtok(textCopy," \t");
		while(token!=NULL){
			code = insertTrie(node,token,array[i].id);
			if(code != OK)
				return code;
			token = strtok(NULL," \t");
		}
		free(textCopy);
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

/* Searches for the given word in the Trie */
postingsList* searchTrie(trieNode* node,char* word){
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

		if(temp == NULL)
			return NULL;
		

		if(value < 0){
			// printf("2)The word '%s' is not in the Trie\n",word);
			return NULL;
		}

		if(i == strlen(word)-1){
			if(temp->isEndOfWord == True){
				// printf("The word '%s' is in the Trie\n",word);
				return temp->listPtr;
			}
			else
				return NULL;
		}

		previous = NULL;
		
	}
	return NULL;
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
		if(strlen(lineptr) == 1)
			continue;			
		lines++;
	}

	free(lineptr);
	return lines;
}


/*********************/
/*** POSTINGS LIST ***/ 
/***   FUNCTIONS   ***/
/*********************/

int addList(postingsList** ptr, int textID, char* theWord){

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
		(*ptr)->word = (char*)malloc(strlen(theWord)+1);
		if((*ptr)->word == NULL)
			return MEMORY_NOT_ALLOCATED;
		strcpy((*ptr)->word,theWord);
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
	free(ptr->word);
	free(ptr);
}

void printPostingsList(postingsList* ptr, char* word){
	if(ptr == NULL){
		printf("The word %s is not in the Trie\n",word);
		return;
	}
	postingsListNode* temp = ptr->headPtr;

	printf("\n******************\n");
	printf("WORD: %s\n",ptr->word);
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

void printAllDF(trieNode* node){
	if(node == NULL)
		return;
	printAllDF(node->next);
	printAllDF(node->children);
	if(node->isEndOfWord == True)
		printf("%s %d\n",node->listPtr->word,node->listPtr->dfVector);
}


/***********************/
/*** SCORE FUNCTIONS ***/
/***********************/
double getIDF(int totalTexts, postingsList* ptr){
	if(ptr == NULL)
		return ERROR;

	postingsListNode* temp = ptr->headPtr;
	double numerator = ((totalTexts)-(ptr->dfVector))+0.5;
	double denominator = (ptr->dfVector+0.5);
	double fraction = numerator/denominator;
	return(log10(fraction));
}

double getScore(postingsListNode* ptr, postingsList* ptr1, int totalTexts,map* array, double avg){

	double numerator = (ptr->occurences)*(1.2+1.0);
	double denominator = (ptr->occurences)+((1.2)*((1-0.75)+(0.75*((double)(array[ptr->textID].words)/avg))));
	double fraction = numerator/denominator;
	double idf = getIDF(totalTexts,ptr1);
	double score = idf*fraction;
	return score;			
}

/***************************/
/*** OPERATION FUNCTIONS ***/
/***************************/

int searchOperation(trieNode* node, char* arguments, int totalTexts, map* array, double avg, int k){

	if(arguments == NULL)
		return ARGUMENTS_ERROR;

	double score;

	scoreList* head = NULL; 					/* Points at the head of the list which holds the scores */
	// trieNode* temp = node;						/* Points at the root of the Trie */
	postingsList* ptr;							/* Points at the node which holds info for the postings list of a word */
	postingsListNode* tempNode;
	char* word;									/* The word we're currently working on */
	int textID;									/* The ID of the text (document) we're currently working on */

	int errorCode;

	word = strtok(arguments," ,.-");
  	while(word != NULL){
    	ptr = searchTrie(node,word);
    	if(ptr != NULL){   	
    		tempNode = ptr->headPtr;
    		while(tempNode != NULL){			/* Traverse the postings list */
    			textID = tempNode->textID;
				score = getScore(tempNode,ptr,totalTexts,array,avg);
				errorCode = addScoreList(&head,textID,score);
				tempNode = tempNode->next;
			}
		}
   		word = strtok(NULL, " ,.-");
  	}

  	scoreList* tmp = head;
  	
  	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int terminalRows = w.ws_row;
	int terminalColumns = w.ws_col;

	int columnCounter = 0;

  	for(int i = 0; i < k; i++){
  		printf("%d.( %d)[%5.4f] ",i,tmp->textID,tmp->score);
  		printf("TEXT: %s\n",array[tmp->textID].text);
  		tmp = tmp->next;
  	}

  	free(head);
  	return OK;
}

int dfOperation(trieNode* node,char* arguments, int totalTexts, map* array){
	postingsList* ptr;
	postingsListNode* tempNode;
	char* word;

	if(arguments != NULL){
		ptr = searchTrie(node,arguments);
		if(ptr != NULL)
			printf("%s %d\n",arguments,ptr->dfVector);		
	}
	else
		printAllDF(node);
	return OK;
}

/****************************/
/*** SCORE LIST FUNCTIONS ***/
/****************************/

int addScoreList(scoreList** ptr, int textID, double score){
	scoreList* temp;
	scoreList* previous = NULL;

	double value;
	double newScore = score;

	if(*ptr == NULL){
		*ptr = (scoreList*)malloc(sizeof(scoreList));
		if(*ptr == NULL)
			return MEMORY_NOT_ALLOCATED;
		(*ptr)->textID = textID;
		(*ptr)->score = newScore;
		(*ptr)->next = NULL;
	}
	else{
		temp = *ptr;
		while(temp != NULL){
			if(temp->textID != textID){
				previous = temp;
				temp = temp->next;
				continue;
			}
			else{
				if(previous == NULL){
					temp->score += newScore;
					return OK;
				}
				else{
					newScore += temp->score;
					previous->next = temp->next;
					free(temp);
					break;
				}
			}
		}

		previous = NULL;
		temp = *ptr;
		while(temp != NULL){
			value = temp->score - newScore;

			if(value > 0){
				previous = temp;
				temp = temp->next;
				continue;
			}
			if(value <= 0)
				break;
		}

		if(value > 0){
			temp = (scoreList*)malloc(sizeof(scoreList));
			if(temp == NULL)
				return MEMORY_NOT_ALLOCATED;
			temp->textID = textID;
			temp->score = newScore;
			temp->next = previous->next;
			previous->next = temp;
		}
		else if(value < 0){
			if(previous != NULL){
				temp = (scoreList*)malloc(sizeof(scoreList));
				if(temp == NULL)
					return MEMORY_NOT_ALLOCATED;
				temp->textID = textID;
				temp->score = newScore;
				temp->next = previous->next;
				previous->next = temp;
			}
			else{
				previous = (scoreList*)malloc(sizeof(scoreList));
				if(previous == NULL)
					return MEMORY_NOT_ALLOCATED;
				previous->textID = textID;
				previous->score = newScore;
				previous->next = temp;
				(*ptr) = previous;
			}
		}
		else if(value == 0){
			previous = temp;
			temp = (scoreList*)malloc(sizeof(scoreList));
			if(temp == NULL)
				return MEMORY_NOT_ALLOCATED;
			temp->textID = textID;
			temp->score = newScore;
			temp->next = previous->next;
			previous->next = temp;
		}

	}

	return OK;
}

void deleteScoreList(scoreList* ptr){
	if(ptr == NULL)
		return;
	deleteScoreList(ptr->next);
	free(ptr);
}