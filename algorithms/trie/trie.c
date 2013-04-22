#include "trie.h"

void rFree(TrieNode *, void (*del) (void *));
TrieNode *allocNode(char key);
_Bool rAdd(TrieNode *node, const char *keys, 
                       void *data, size_t start, size_t end,
                       int (*hash)(char));
void *rGet(TrieNode *node, const char *keys, 
                       size_t start, size_t end, int (*hash) (char));
void rPrint(TrieNode *node, void (*prnt)(void*));
_Bool rUpdate(TrieNode *node, const char *keys, size_t start, size_t end, 
                                                void (*update) (void *),
                                                int (*hash) (char));


TrieNode *allocNode(char key) {
    TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
    if (newNode) {
        for (int i=0; i<MAX_DICT; ++i) { newNode->links[i] = NULL; }
        newNode->key = key;
    }
    return newNode;
}

Trie *trie_alloc(int (*hash) (char), void (*del)(void *), void (*prnt)(void *)) {
    Trie *newTrie = (Trie *)malloc(sizeof(Trie));
    if (newTrie) {
        newTrie->root = allocNode('\0');
        newTrie->hash = hash;
        newTrie->del = del;
        newTrie->prnt = prnt;
        newTrie->size = 0;
    }
    return newTrie;
}

void trie_free(Trie *trie) {
    rFree(trie->root, trie->del);
    free(trie);
}

_Bool trie_update(Trie *trie, const char *keys, void (*update) (void *)) {
    if (trie->root) {
        size_t len = strlen(keys);
        return rUpdate(trie->root, keys, 0, len, update, trie->hash);
    }
    return false;
}

_Bool rUpdate(TrieNode *node, const char *keys, size_t start, size_t end, 
                    void (*update) (void *), int (*hash) (char)) {
    if (start == end) {
        if (node->data) {
            update(node->data);
            return true;
        } else {
            return false;
        }
    }
    
    if (node->links[hash(keys[start])])
        return rUpdate(node->links[hash(keys[start])], keys, start+1, 
                                        end, update, hash);
    else
        return false;
}

void trie_print(Trie *trie) {
    if (trie->prnt)
        rPrint(trie->root, trie->prnt);
    else
        fprintf(stderr, "Error: trie's print function is undefined\n");
}

_Bool trie_insert(Trie *trie, const char *keys, void *data) {
    size_t len = strlen(keys);
    if (len > 0) {
        _Bool res = rAdd(trie->root, keys, data, 0, len, trie->hash);
        if (res) trie->size++;
        return res;
    }
    return false;
}

void *trie_lookup(Trie *trie, const char *keys) {
    size_t len = strlen(keys);
    return rGet(trie->root, keys, 0, len, trie->hash);
}


void rPrint(TrieNode *node, void (*prnt)(void*)) {
    for (int i=0; i < MAX_DICT; ++i)
        if (node->links[i]) 
            rPrint(node->links[i], prnt);
    if (node->data)
        prnt(node->data);
}

_Bool rAdd(TrieNode *node, const char *keys, 
                       void *data, size_t start, size_t end,
                       int (*hash)(char)) {
    if (start == end) {
        node->data = data; // assign the data
        return true;
    }
    
    if (node->links[hash(keys[start])] == NULL) {
        node->links[hash(keys[start])] = allocNode(hash(keys[start]));
        if (node->links[hash(keys[start])] == NULL) // error check
            return false;
    }
    //recursive call
    return rAdd(node->links[hash(keys[start])], 
                        keys, data, start+1, end, hash);
}

void *rGet(TrieNode *node, const char *keys, 
                       size_t start, size_t end, int (*hash) (char)) {
    if (start == end)
        return node->data;

    if (node->links[hash(keys[start])] != NULL)
        return rGet(node->links[hash(keys[start])], keys, start+1, end, hash);
    else
        return NULL;
}

void rFree(TrieNode *node, void (*del) (void *)) {
    for (int i=0; i < MAX_DICT; ++i)
        if (node->links[i]) 
            rFree(node->links[i], del);
    if (node->data && del)
        del(node->data);
}
