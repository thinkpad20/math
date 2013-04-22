//
//  btrie.h
//  BTrie
//
//  Created by Allen Nelson on 4/22/13.
//  Copyright (c) 2013 Allen Nelson. All rights reserved.
//

#ifndef BTrie_btrie_h
#define BTrie_btrie_h

#include <stdlib.h>
#include <stdbool.h>

/* links stored in binary tree */
struct BTrieLink {
    char c;
    struct BTrieNode *node;
    struct BTrieLink *children[2];
};

struct BTrieNode {
    void *data;
    struct BTrieLink *links;
};

struct BTrie {
    size_t size;
    struct BTrieNode *root;
};

void btrie_init(struct BTrie *t);
void *btrie_insert(struct BTrie *t, void *ptr);
void *btrie_lookup(struct BTrie *t, void *ptr);

#endif
