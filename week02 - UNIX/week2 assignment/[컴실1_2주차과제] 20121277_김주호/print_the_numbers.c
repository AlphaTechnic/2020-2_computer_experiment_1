#include "Header.h"

void print_the_numbers(int* ans) {
	int l;

	for (l = 0; l < 10; l++) {
		printf("%d ", ans[l]);
		ans[l] = 0;
	}
}
