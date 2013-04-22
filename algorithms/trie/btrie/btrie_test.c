//
//  btrie_test.c
//  PTrie
//
//  Created by Allen Nelson on 4/22/13.
//  Copyright (c) 2013 Allen Nelson. All rights reserved.
//

#include <stdio.h>
#include "btrie.h"

typedef struct PTrie PTrie;

int main(int argc, const char * argv[])
{
    const char *strings[] = {
        "hello",
        "hi",
        "how are you?",
        "I'm fine thanks" };
    
    int ints[] = {1, 2, 3, 4, 5, 234, 456, 23, 678, 98457,
                  230498, 20394, 39, 48, 10};
    PTrie pt;
    ptrie_init(&pt);
    for (int i=0; i<4; ++i) {
        printf("inserting %p (%s)\n", strings[i], strings[i]);
        ptrie_insert(&pt, (void *)strings[i]);
    }
    
    for (int i=0; i<20; ++i) {
        printf("inserting %p (%d)\n", ints + i, ints[i]);
        ptrie_insert(&pt, (void *)(ints + i));
    }
    
    for (int i=0; i<4; ++i) {
        void *look = (void *)strings[i];
        char *temp = ptrie_lookup(&pt, look);
        printf("lookup result: %p (%s)\n", temp, temp);
    }
    
    for (int i=0; i<20; ++i) {
        printf("lookup result: %p (%d)\n", ptrie_lookup(&pt, (void *)(ints + i)),
               *((int *)ptrie_lookup(&pt, (void *)(ints + i))));
    }
    return 0;
}

