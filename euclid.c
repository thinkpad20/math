#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int size;
	int* array;
} Vector;

int vector_pop(Vector *s) {
	int nReturn;
	nReturn = s->array[s->size];
	s->size--;
	return nReturn;
}

void unwindEuclid(Vector *s, int a, int b, int gcd, int* res) {
	int w, x, y, z,
		next_l, next_q, next_m, next_r, j = 0;
	/* remove the first four elements, we don't care about those */
	w = vector_pop(s);
	z = vector_pop(s);
	y = vector_pop(s);
	x = vector_pop(s);
	vector_pop(s);
	w = 1;
	z = vector_pop(s);
	y = vector_pop(s);
	x = vector_pop(s);
	printf ("%d = %d * %d - %d * %d\n", gcd, w, x, y, z);
	if ((x == a && z == b) || (x == b && z == a) 
			|| (w == a && y == b) || (w == b && y == a)) 
		return;

	while (++j <8) {
		/* get next data */
		next_r = vector_pop(s);
		next_m = vector_pop(s);
		next_q = vector_pop(s);
		next_l = vector_pop(s);
		/*printf("next_l = %d, next_q = %d, next_m = %d, next_r = %d\n", next_l, next_q, next_m, next_r);*/
		/*substitute for z*/
		w += y * next_q;
		z = next_l;
		printf ("%d = %d * %d - %d * %d\n", gcd, w, x, y, z);
		if ((x == a && z == b) || (x == b && z == a) || (w == a && y == b) || (w == b && y == a)) break;
		/* get next data */
		next_r = vector_pop(s);
		next_m = vector_pop(s);
		next_q = vector_pop(s);
		next_l = vector_pop(s);
		/*printf("next_l = %d, next_q = %d, next_m = %d, next_r = %d\n", next_l, next_q, next_m, next_r);*/
		/* substitute for x */
		x = next_l;
		y += w * next_q;
		printf ("%d = %d * %d - %d * %d\n", gcd, w, x, y, z);
		/*printf("w = %d, a = %d, y = %d, b = %d", w, a, y, b);*/
		if ((x == a && z == b) || (x == b && z == a) || (w == a && y == b) || (w == b && y == a)) break;
	}

	if (x==a) res[0] = w;
	if (x==b) res[1] = w;
}

void print_array(int* array, size_t len) {
	int i;
	printf("(");
	for (i=0; i<len; i++)
		printf("%d, ", array[i]);
	printf("\b\b)\n");
}

/* Uses Euclid's algorithm to find gcd(a,b) */
int euclids_algorithm(int a, int b) {
	/* Set up n = q*m + r */
	int left, right_q, right_m, right_r, coeffs[2];
	/* Create vector to store each n, q, m and r in */
	Vector *s;
	s = malloc(sizeof(Vector));
	s->array = malloc(200*sizeof(int));
	s->size = 0;

	/* Put the greater of a and b on the left side*/
	left = a>b?a:b;
	right_m = a<b?a:b;
	right_q = left/right_m;
	right_r = left % right_m;
	printf("%d = %d * %d + %d\n", left, right_q, right_m, right_r);

	/* Insert elements in vector */
	s->array[0] = left;
	s->array[++s->size] = right_q;
	s->array[++s->size] = right_m;
	s->array[++s->size] = right_r;
	while (right_r) {
		left = right_m;
		right_m = right_r;
		right_q = left/right_m;
		right_r = left % right_m;
		printf("%d = %d * %d + %d\n", left, right_q, right_m, right_r);
		s->array[++s->size] = left;
		s->array[++s->size] = right_q;
		s->array[++s->size] = right_m;
		s->array[++s->size] = right_r;
	}
	printf("gcd(%d,%d) = %d\n", a, b, right_m);
	printf("Expressing %d as a linear combination of %d and %d:\n", right_m, a, b);
	unwindEuclid(s, a, b, right_m, coeffs);
	return right_m;
}

int main(int argc, char* argv[]) {
	int gcd;
	if (argc != 3) {
		printf("Error: please enter two integer arguments.\n");
		exit(0);
	}
	gcd = euclids_algorithm(atoi(argv[1]), atoi(argv[2]));
	return 0;
}
