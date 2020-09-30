# Keywords Search Engine

A mini search engine implemented using the C programming language. 

The implementation of this project was part of the coursework for the [National and Kapodistrian University of Athens](https://en.uoa.gr/), [Department of Informatics and Telecommunications](https://www.di.uoa.gr/en) [Systems Programming](http://cgi.di.uoa.gr/~antoulas/k24/) undergraduate course.

## What is it?

The main functionality of the program is that it acts as a search engine that responds to keyword queries from a single user. These keywords are then traced back to a series of texts located inside a document that the user must have provided before the start of the program's execution. Whenever a word is found inside one text, that entire text is returned and displayed on the user's screen alongside a unique number that identifies it and its [relevance score](https://en.wikipedia.org/wiki/Okapi_BM25).

The program's operations also include calculating a document's frequence vector and a word's term frequency.

## How does it work?

At the beginning of its execution, the program reads a series of texts from a user-provided document and stores them in memory. Specifically, it places them in a [dynamic array](https://en.wikipedia.org/wiki/Dynamic_array), that's created at runtime, along with an ID and their word count. Afterward, an [inverted index](https://en.wikipedia.org/wiki/Inverted_index) is implemented using a [Trie](https://en.wikipedia.org/wiki/Trie) data structure, in which every word that has been encountered is stored. 

All nodes of the above tree structure hold a single character that might be the last letter of a word. If so, there's a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) variable that leads to a [linked list](https://en.wikipedia.org/wiki/Linked_list), where each node holds the ID of a text that the word appears in and the number of its occurrences in that text.

Whenever a user searches for a certain keyword, the program traverses the trie structure looking for it. If the keyword is found, the program moves on to the linked list and stores the IDs of the texts in descending order in a new list, based on the word's relevance score.

Once all the keywords have been searched, the top *K* results are displayed on the user's screen, where *K* is a user-defined integer given at the start of the program.

## What technologies were used?

- C
- Inverted index
- Trie
- Linked list

## How do I run it?

Before we start running the program, we need to compile all its files. For that purpose a [makefile](https://en.wikipedia.org/wiki/Makefile) has been created and we execute it by typing the following on the command prompt:

```bash
make
```

 Now we're ready to begin. To start the program we can use either one of the following commands:

```bash
./minisearch -i docfile -k K
```

```bash
./minisearch -k K -i docfile
```

```bash
./minisearch -i docfile
```

where *docfile* is the name of the file that holds the texts we'll be using and *K* the number of the top results that we want to be displayed on the screen.

We are then greeted by a message that informs us of the operations that the program is capable of.

![Welcome screen](https://github.com/frinipanteliadi/Keywords-Search-Engine/blob/master/Images/Welcome%20screen.png)

### Searching for keywords

We can search for certain keywords in the provided texts by using the */search* operation. To do that we type:

```
/search q1 q2 q3 q4 ... q4
```

Keep in mind that:

- at least one keyword must be given at all times,
- if more than 10 keywords are given, only the first 10 will be searched for. 



The search results will look something like this:

![Search results](https://github.com/frinipanteliadi/Keywords-Search-Engine/blob/master/Images/Search%20results.png)

where the number enclosed in parentheses is the ID of the text and the number enclosed in square brackets is the relevance score.

