#include "Header.h"

void register_TestCase_at_dynamic_array(int* arr_dynamic, int t) {
	int i;

	printf("input pages of the books : \n");
	for (i = 0; i < t; i++) {
		scanf("%d", &arr_dynamic[i]);
	}
}
