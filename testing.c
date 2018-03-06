typedef struct trieNode{
	char letter;					/*Letter stored inside the node*/
	bool isEndOfWord;				/*Flag which indicated whether a word ends in the current node*/
	struct trieNode **children;		/*An array of pointers, where each one will store the address of a child node*/
	postingsList *listPtr;			/*Pointer to postings list of a leaf node*/
	int arraySize;					/*Size of the array of pointers (needed for use with realloc)*/
}trieNode;


/* Creading the root of the Trie data structure */
int createRoot(trieNode *root){
	
	/* Create the root of the Trie */
	root = (trieNode*)malloc(sizeof(trieNode));
	if(root == NULL)
		return -1;

	/* Initialize the root */
	root->letter = 0;
	root->isEndOfWord = False;
	root->listPtr = NULL;
	root->arraySize = 10;

	root->children = (trieNode**)malloc((root->arraySize)*sizeof(trieNode*));
	if(root->children == NULL)
		return -1;

	/* Initialize the array for the children with NULL */
	for(int i=0; i<(root->arraySize); i++)
		root->children[i] = NULL;
}

/* Compares the ASCII value between two characters.
   Helps with finding the right position in the Trie.

   Possible return values: -1, 0, 1 */
int compareKeys(char* a, char* b){
	
	

}

int insertTrie(trieNode* node, char* word){
	
	trieNode* temp;
	temp = node;

	for(int i=0; i<strlen(word); i++){
		for(int j=0; j<(temp->arraySize); j++){
		
			int comparison = compareKeys(word[i],(temp->children[j]));
			if(comparison == -1){}
			else if(comparison == 0){}
			else if(comparison == 1){}
		}
	}
}