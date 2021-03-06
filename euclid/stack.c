#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void stack_resize(Stack *s, size_t size); /* internal use only */

void stack_init(Stack * s, unsigned long size) {
	s->size = 0;
	s->maxSize = size;
	s->array = malloc(sizeof(_TYPE_) * size);
	if (!s->array) {
		fprintf(stderr, "Error: memory allocation failure\n");
		exit(1);
	}
}

_TYPE_ stack_pop(Stack *s) {
	_TYPE_ retval = s->array[s->size-1];
	s->size--;
	if (s->size <= s->maxSize/2)
		stack_resize(s, s->maxSize/2);
	return retval;
}

void stack_push(Stack *s, _TYPE_ val) {
	if (s->size == s->maxSize)
		stack_resize(s, s->maxSize * 2);
	s->array[s->size] = val;
	s->size++;
}

void stack_resize(Stack *s, size_t size) {
	_TYPE_ *temp = realloc(s->array, sizeof(_TYPE_) * size);
	if (!temp) {
		fprintf(stderr, "Error: memory allocation failure\n");
		exit(1);
	}
	s->array = temp;
	s->maxSize = size;
}

void stack_delete(Stack *s) {
	free(s->array);
	free(s);
}