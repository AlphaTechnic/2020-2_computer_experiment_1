#include "Header.h"

void get_num_of_TestCase(int* t) {
	// ����ڷκ��� testcase�� ���� t�� �Է¹޴´�. 
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

