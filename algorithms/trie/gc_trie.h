#ifndef __GC_TRIE_H__
#define __GC_TRIE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DICT 256

typedef struct Trie_ {
	char key;
	void *data;
	struct Trie_ *subTries[MAX_DICT]; 
} Trie;

Trie *trie_init(); /*Initializes a trie with a null key. Good for a first node.*/
Trie *trie_initWithKey(char); /*Initializes a trie with a specified key.*/
void trie_printElement(Trie *trie); /* prints a Trie (somewhat inelegantly) */
void trie_addElement(Trie *trie, const char *keys, void *data); /* Adds an element. */
void trie_getElement(Trie *trie, const char *keys, void **dest); /* Repoints dest at the data 
																    stored in the element. */

#endif /* __GC_TRIE_H__ */