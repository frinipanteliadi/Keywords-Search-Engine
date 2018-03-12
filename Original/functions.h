/* False = 0, True = 1 */
typedef enum bool {False, True} bool;

typedef struct map{
	int id;							/*ID of the text which is stored*/
	char *text;						/*The actual text*/
}map;

typedef struct postingsListNode{
	int textID;						/*ID of the text*/
	int occurences;					/*Number of occurences in a specific text*/
	struct postingsListNode *next;	/*Pointer to the next node of the postings list*/
}postingsListNode;

typedef struct postingsList{
	int df_vector;					/*Total number of texts in which the word appears*/
	postingsListNode* ptr;			/*Pointer to the head of the postings list*/
}postingsList;

typedef struct trieNode{
	char letter;					/*Letter stored inside the node*/
	bool isEndOfWord;				/*Flag which indicated whether a word ends in the current node*/
	struct trieNode **children;		/*An array of pointers, where each one will store the address of a child node*/
	postingsList *listPtr;		    /*Pointer to postings list of a leaf node*/
	int arraySize;					/*Size of the array of pointers (needed for use with realloc)*/
	int childNodes;					/*Indicates the number of children the node has*/
}trieNode;

int initializeMap(FILE*, char*, char*, map*);
void printMap(int, map*);
int compareKeys(char*, char*);
void initializeRoot(trieNode **);
void printNode(trieNode *);
void printNodes(trieNode *);
void destroyTrie(trieNode*);
void insertTrie(trieNode*, char*);
void initializeTrie(int, char *, trieNode*, map*);
int getNumberOfLines(FILE*, char* lineptr);