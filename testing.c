typedef struct trieNode{
	char letter;					/*Letter stored inside the node*/
	bool isEndOfWord;				/*Flag which indicated whether a word ends in the current node*/
	struct trieNode **children;		/*An array of pointers, where each one will store the address of a child node*/
	// postingsList *listPtr;		/*Pointer to postings list of a leaf node*/
	int arraySize;					/*Size of the array of pointers (needed for use with realloc)*/
	int childNodes;					/*Indicates the number of children the node has*/
}trieNode;


/* Initializing all members of the root */
void initializeRoot(trieNode **root){

	(**root).letter = 0;
	(**root).isEndOfWord = False;
	// (**root).listPtr = NULL;
	(**root).arraySize = 0;
	(**root).childNodes = 0;
	(**root).children = NULL;
}

/* Returns the difference between the ASCII values
   of a and b */
int compareKeys(char* a, char* b){
	return(((*a)-(*b)));
}	

/* Inserts a new value (word) in the Trie */
int insertTrie(trieNode* node, char* word){
	trieNode* temp = node;

	/* Each iteration is responsible for a single
	   letter of the word we're trying to insert */
	for(int i=0; i < strlen(word); i++){

		/* The node we're currently working on doesn't have
		   any children */
		if(temp->children == NULL){
			temp->arraySize = 10;
			temp->childNodes++;

			temp->children = (trieNode**)malloc((temp->arraySize)*sizeof(trieNode*));
			if(temp->children == NULL)
				return -1;

			for(int j=0; j<(temp->arraySize); j++){
				if(j>0)
					temp->children[j] = NULL;
				else{
					temp->children[0] = (trieNode*)malloc(sizeof(trieNode));
					if(temp->children[0] == NULL)
						return -1;

					temp->children[0]->letter = word[i];

					/* Checking whether the current letter is the final one
					   of a word */
					if(i<(strlen(word)-1)){
						temp->children[0]->isEndOfWord = False;
						// temp->children[0]->listPtr = NULL;
					}
					else{
						temp->children[0]->isEndOfWord = True;
						// temp->children[0]->listPtr points to the head of a postings list
					}

					temp->children[0]->arraySize = 0;
					temp->children[0]->childNodes = 0;
					temp->children[0]->children = NULL;
				}
			}
			
			/* If there are still some letters left, we have
			   to make the pointer point at the node we 
			   recently created */
			if(i < strlen(word)-1){
				temp = temp->children[0];
				continue;
			}
		}
		
		/* The node we're currently on has at least
		   one child */
		else{
			
			/* We need to find the correct placement 
			   for the letter we are trying to insert */
			int j = 0;
			int value = 1;
			while((value!=0) && j<(temp->childNodes)){
				value = compareKeys(word[i],(temp->children[j]->letter));
				if(value/*value < 0 || value > 0*/)
					j++;
			}
			/*for(j=0; j < (temp->childNodes); j++){
				int value = compareKeys(&(temp->children[j]->letter),&word[i]);

				if(value == -1)
					continue;
				else if(value == 1)
			}*/

			/* The node already exists */
			if(temp->children[j] == NULL && temp->childNodes != temp->arraySize){
				temp->childNodes++;	

				temp->children[j] = (trieNode*)malloc(sizeof(trieNode));
				if(temp->children[j] == NULL)
					return -1;

				temp->children[j]->letter = word[i];

				/* The letter we just inserted does not
				   indicate the end of a word */
				if(i < (strlen(word)-1)){
					temp->children[j]->isEndOfWord = False;
					// temp->children[j]->listPtr = NULL;
				}

				/* The current node doesn't have any children.
				   That's why we must initialize the following 
				   values with zero */
				temp->children[j]->arraySize = 0
				temp->children[j]->childNodes = 0;
				temp->children[j]->children = NULL;
			}

			/* If we still have letters that we need to insert,
			   we must change the node that we're working on */
			if(i < strlen(word)-1){
				temp = temp->children[j];
				continue;
			}
		}
	}

	return 0;
}