#include "Header.h"

void get_num_of_TestCase(int* t) {
	// 사용자로부터 testcase의 갯수 t를 입력받는다. 
	int check = 1;

	while (check) {
		if (*t < 0) {
			printf("please input inter greater than 0 : \n");
			scanf("%d", t);
		}
		else {
			check = 0;
		}
	}
}

