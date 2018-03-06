typedef struct trieNode{
	char letter;					/*Letter stored inside the node*/
	bool isEndOfWord;				/*Flag which indicated whether a word ends in the current node*/
	struct trieNode **children;		/*An array of pointers, where each one will store the address of a child node*/
	postingsList *listPtr;			/*Pointer to postings list of a leaf node*/
	int arraySize;					/*Size of the array of pointers (needed for use with realloc)*/
}trieNode;


/* Creating the root of the Trie data structure */
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


int insertTrie(trieNode* node, char* word){
	trieNode* temp;
	temp = node;

	/* We need to place each one of the letters 
	   in different nodes. Some of the letters
	   might already be in the Trie. In that case,
	   we will simply not insert them again */
	for(int i=0; i<strlen(word); i++){
		/* Traverse the array which holds the pointers
		   for the children */
		for(int j=0; j<(temp->arraySize); j++){
			if(temp->children[j] == NULL){
				temp->children[j] = (trieNode*)malloc(sizeof(trieNode));
				if(temp->children[j] == NULL)
					return -1;
				temp->children[j]->letter = word[i];
			}
		}
	}
}