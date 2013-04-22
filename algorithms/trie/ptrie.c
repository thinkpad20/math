#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ptrie.h"

#define false 0
#define true 1

void ptrie_freeR(PTrieNode *, void (*del) (void *));
PTrieNode *allocNode(char key);
int ptrie_insertR(PTrieNode *node, void *data, size_t start, size_t end);
void *ptrie_lookupR(PTrieNode *node, void *data, size_t start, size_t end);
void ptrie_printR(PTrieNode *node, void (*prnt)(void*));
int num_nodes = 0;

PTrieNode *allocNode(char key) {
    num_nodes++;
    PTrieNode *newNode = (PTrieNode *)malloc(sizeof(PTrieNode));
    if (newNode) {
        for (int i=0; i<MAX_DICT; ++i) { newNode->links[i] = NULL; }
        newNode->key = key;
    }
    return newNode;
}

void ptrie_init(PTrie *newPTrie, void (*del)(void *), void (*prnt)(void *)) {
    if (newPTrie) {
        newPTrie->root = allocNode('\0');
        newPTrie->del = del;
        newPTrie->prnt = prnt;
        newPTrie->size = 0;
    }
}

void ptrie_free(PTrie *trie) {
    ptrie_freeR(trie->root, trie->del);
    free(trie);
}

void ptrie_print(PTrie *trie) {
    if (trie->prnt)
        ptrie_printR(trie->root, trie->prnt);
    else
        fprintf(stderr, "Error: trie's print function is undefined\n");
}

void *ptrie_lookup(PTrie *trie, void *data) {
    size_t len = sizeof(data);
    return ptrie_lookupR(trie->root, data, 0, len);
}


void ptrie_printR(PTrieNode *node, void (*prnt)(void*)) {
    for (int i=0; i < MAX_DICT; ++i)
        if (node->links[i]) 
            ptrie_printR(node->links[i], prnt);
    if (node->data)
        prnt(node->data);
}

int ptrie_insert(PTrie *trie, void *data) {
    size_t len = sizeof(data);
    if (len > 0) {
        int res = ptrie_insertR(trie->root, data, 0, len);
        if (res) trie->size++;
        return res;
    }
    return false;
}

int ptrie_insertR(PTrieNode *node, void *data, int start, int end) {
    if (start == end) {
        node->data = data; // assign the data
        return true;
    }
    
    // grab the start'th byte of data
    long tmp = (long) data << (start * 8);
    int key = tmp & 0xff00000000000000;

    // see if we have it in our link table
    if (node->links[key] == NULL) {
        node->links[key] = allocNode(key);
        if (node->links[key] == NULL) // error check
            return false;
    }

    //recursive call
    return ptrie_insertR(node->links[key], data, start+1, end);
}

void *ptrie_lookupR(PTrieNode *node, void *data, size_t start, size_t end) {
    if (start == end)
        return node->data;

    // grab the start'th byte of data
    long tmp = (long)data << (start * 8);
    int key = tmp & 0xff00000000000000;

    if (node->links[key] != NULL)
        return ptrie_lookupR(node->links[key], data, start+1, end);
    else
        return NULL;
}

void ptrie_freeR(PTrieNode *node, void (*del) (void *)) {
    for (int i=0; i < MAX_DICT; ++i)
        if (node->links[i]) 
            ptrie_freeR(node->links[i], del);
    if (node->data && del)
        del(node->data);
}
