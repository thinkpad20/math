#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int gcd(int a, int b);
/* returns gcd(a, b) */

int euclidZip(int a, int b);
/* Computes gcd(a,b) and stores the data that feeds into euclidUnzip */

void euclidUnzip(Stack *s, int a, int b, int gcd, int* res);
/* Computes x and y such that x*a + y*b = gcd(a,b) */

int gcd(int a, int b) {
	int left, right_q, right_m, right_r;
	if (a <= 0 || b <= 0) {
		fprintf(stderr, "Error: both inputs must be positive.\n");
		exit(1);
	}
	/* Put the greater of a and b on the left side */
	left = a>b?a:b;
	right_m = a<b?a:b;
	right_q = left/right_m;
	right_r = left % right_m;
	while (right_r != 0) {
		left = right_m;
		right_m = right_r;
		right_q = left/right_m;
		right_r = left % right_m;
	}
	return right_m;
}

int euclidZip(int a, int b) {
	/* Set up n = q*m + r */
	int left, right_q, right_m, right_r, coeffs[2];
	Stack *s;

	if (a <= 0 || b <= 0) {
		printf("Error: both inputs must be positive.\n");
		exit(1);
	}
	/* Create stack to store each n, q, m and r in */
	s = malloc(sizeof(Stack));
	stack_init(s, 16);

	/* Put the greater of a and b on the left side*/
	left = a>b?a:b;
	right_m = a<b?a:b;
	right_q = left/right_m;
	right_r = left % right_m;
	printf("%d = %d * %d + %d", left, right_q, right_m, right_r);

	/* Insert elements in stack */
	stack_push(s, left);
	stack_push(s, right_q);
	stack_push(s, right_m);
	stack_push(s, right_r);

	while (right_r != 0) {
		left = right_m;
		right_m = right_r;
		right_q = left/right_m;
		right_r = left % right_m;
		printf("\n%d = %d * %d + %d", left, right_q, right_m, right_r);
		stack_push(s, left);
		stack_push(s, right_q);
		stack_push(s, right_m);
		stack_push(s, right_r);
	}
	printf("  <== gcd(%d, %d) = %d\n", a, b, right_m);
	euclidUnzip(s, a, b, right_m, coeffs);

	stack_delete(s);
	return right_m;
}

void euclidUnzip(Stack *s, int a, int b, int gcd, int* res) {
	int w = 1, x, y, z, next_l, next_q, next_m, next_r;
	printf("Expressing %d as a linear combination of %d and %d:\n", gcd, a, b);
	/* remove the first five elements, we don't care about those */
	for (x = 0; x < 5; ++x) stack_pop(s);
	z = stack_pop(s);
	y = stack_pop(s);
	x = stack_pop(s);
	printf ("%d = %d * %d - %d * %d\n", gcd, w, x, y, z);
	if ((x == a && z == b) || (x == b && z == a) 
			|| (w == a && y == b) || (w == b && y == a)) 
		return;

	while (1) {
		/* get next data */
		next_r = stack_pop(s);
		next_m = stack_pop(s);
		next_q = stack_pop(s);
		next_l = stack_pop(s);
		/*substitute for z*/
		w += y * next_q;
		z = next_l;
		printf ("%d = %d * %d - %d * %d\n", gcd, w, x, y, z);
		if ((x == a && z == b) || (x == b && z == a) || 
			(w == a && y == b) || (w == b && y == a)) 
			break;
		/* get next data */
		next_r = stack_pop(s);
		next_m = stack_pop(s);
		next_q = stack_pop(s);
		next_l = stack_pop(s);
		/* substitute for x */
		x = next_l;
		y += w * next_q;
		printf ("%d = %d * %d - %d * %d\n", gcd, w, x, y, z);
		if ((x == a && z == b) || (x == b && z == a) || 
			(w == a && y == b) || (w == b && y == a)) 
			break;
	}

	if (x==a) res[0] = w;
	if (x==b) res[1] = w;
}

int main(int argc, char* argv[]) {
	int a, b;
	if (argc != 3) {
		printf("Error: please enter two integer arguments.\n");
		exit(0);
	}
	a = atoi(argv[1]); b = atoi(argv[2]);	
	printf("gcd(%d, %d) = %d\nSteps:\n", a, b, gcd(a,b));
	euclidZip(a, b);
	return 0;
}
