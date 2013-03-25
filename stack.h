#ifndef __STACK_H__
#define __STACK_H__

/* set the desired type here */
#define _TYPE_ int

typedef struct {
	unsigned long size, maxSize;
	_TYPE_ *array;
} Stack;

void stack_init(Stack *s, unsigned long size); /* run this before any operations */
_TYPE_ stack_pop(Stack *s);
void stack_push(Stack *, _TYPE_ val);

#endif