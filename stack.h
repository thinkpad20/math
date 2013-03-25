#ifndef __STACK_H__
#define __STACK_H__

/* set the desired type here */
#define _TYPE_ int

typedef struct {
	int size, maxSize;
	_TYPE_ *array;
} Stack;

void stack_init(Stack *, _TYPE_); /* run this before any operations */
_TYPE_ stack_pop(Stack *);
void stack_resize(Stack *, _TYPE_);
void stack_push(Stack *, _TYPE_);

#endif