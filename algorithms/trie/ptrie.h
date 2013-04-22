#ifndef __PTRIE_H__
#define __PTRIE_H__

#define MAX_DICT 256

typedef struct PTrieNode_ {
    char key;
    void *data;
    struct PTrieNode_ *links[MAX_DICT]; 
} PTrieNode;

typedef struct PTrie {
    PTrieNode *root;
    unsigned long size;
    void (*del) (void *); // deletes the object, if set
    void (*prnt) (void *); // prints the object, if set
} PTrie;

void ptrie_init(PTrie *trie, void (*del)(void *), void (*prnt)(void *));
void ptrie_free(PTrie *trie);
void ptrie_print(PTrie *trie); // prints a Trie (somewhat inelegantly)
int ptrie_insert(PTrie *trie, void *data); // Adds an element, returns 1 on success, else 0
void *ptrie_lookup(PTrie *trie, void *data); // returns data if in trie, or null

#endif
