#ifndef __INTSTACK_H__
#define __INTSTACK_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int size, maxSize;
	int* array;
} IntStack;

void intStack_init(IntStack *, int); /* run this before any operations */
int intStack_pop(IntStack *);
void intStack_resize(IntStack *, int);
void intStack_push(IntStack *, int);

#endif