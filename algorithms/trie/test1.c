#include <stdio.h>
#include <stdlib.h>

int main() {
	int *vars[20], i, j;
	int chars[20][8];
	for (j=0; j < 20; ++j) {
		vars[j] = (int *)malloc(sizeof(int));
		unsigned long p = (unsigned long)vars[j];
		int d;
		printf("p = %lx\n", p);
		for (i = 0; i < sizeof(void *); ++i) {
			unsigned long mask = 0xff;
			mask <<= 8*(sizeof(void*) -1 - i);
			printf("d=%x\n", d % 256);
			// mask >>= 8;
			d = (p & mask) >> 8*(8-i-2);
			chars[j][i] = d;
		}
	}
	return 0;
}