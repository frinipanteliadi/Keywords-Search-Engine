#define OK 0
#define NOT_ENOUGH_ARGUMENTS -1
#define FILE_NOT_OPEN -2
#define MEMORY_NOT_ALLOCATED -3
#define WRONG_ORDER -4
#define EXIT -5
#define ERROR -6

/* False = 0, True = 1 */
typedef enum bool {False, True} bool;

typedef struct map{
	int id;							/*ID of the text which is stored*/
	char *text;						/*The actual text*/
}map;

typedef struct postingsListNode{
	int textID;						/*ID of the text*/
	int occurences;					/*Number of occurences inside the text*/
	struct postingsListNode *next;	/*Pointer at the next node of the postings list*/
}postingsListNode;

typedef struct postingsList{
	int dfVector;					/*Total number of texts in which the word appears*/
	postingsListNode* headPtr;	    /*Pointer at the head of the postings list*/
}postingsList;

typedef struct trieNode{
	char letter;					/*Letter stored inside the node*/
	bool isEndOfWord;				/*Flag which indicated whether a word ends in the current node*/
	struct trieNode* children;		/*Points at a child node*/
	struct trieNode* next;			/*Points at the next sibling node*/
	postingsList* listPtr;		    /*Points at the node which holds information regarding the postings list*/
}trieNode;

int compareKeys(char*, char*);
void printError(int);
int initializeMap(FILE*, map*, int);
void printMap(int, map*);
void initializeRoot(trieNode **);
void printNode(trieNode *);
void printNodes(trieNode *);
int insertTrie(trieNode*, char*,int);
int initializeTrie(int, trieNode*, map*);
void destroyTrie(trieNode*);
void searchTrie(trieNode*,char*);
int getNumberOfLines(FILE*);
int addList(postingsList**, int);
void deleteList(postingsListNode*);
void deletePostingsList(postingsList*);
void printPostingsList(postingsList*,char*);