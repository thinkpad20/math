#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DICT 42

typedef struct TrieNode_ {
    char key;
    void *data;
    struct TrieNode_ *links[MAX_DICT]; 
} TrieNode;

typedef struct Trie {
    TrieNode *root;
    size_t size;
    int (*hash) (char); // converts a char to an int i, 0 <= i <= MAX_DICT
    void (*del) (void *); // deletes the object, if set
    void (*prnt) (void *); // prints the object, if set
} Trie;

Trie *trie_alloc(int (*hash) (char), void (*del)(void *), void (*prnt)(void *));
void trie_free(Trie *trie);
void trie_print(Trie *trie); // prints a Trie (somewhat inelegantly)
_Bool trie_insert(Trie *trie, const char *keys, void *data); // Adds an element.
void *trie_lookup(Trie *trie, const char *keys); // returns a pointer
                                                 // to the data if it
                                                 // is in the trie,
                                                 // otherwise NULL
_Bool trie_update(Trie *trie, const char *keys, void (*update) (void *));
// Applies update to the relevant node and returns true if it exists, else false

#endif
