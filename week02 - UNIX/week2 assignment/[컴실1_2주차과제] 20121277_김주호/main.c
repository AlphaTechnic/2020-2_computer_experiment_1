#include "Header.h" 

int main() {
	int t;   // testcase�� ������ �޴� ����
	int i, j, k;        // for���� ��Ʈ���ϴ� ����  
	int check = 1;   // while�� ���Կ��θ� ��Ʈ���ϴ� ����
	int ans[10] = { 0, };   // ���� ��� ����� �ԷµǴ� �迭 
	int* arr_dynamic;

	// ����ڷκ��� testcase�� ���� t�� �Է¹޴´�. 
	printf("please input number of Test Case : ");
	scanf("%d", &t);
	get_num_of_TestCase(&t);

	// �Է����� ������ å�� ������ ���� arr_dynamic�� ����
	arr_dynamic = (int *)malloc(t * sizeof(int));
	register_TestCase_at_dynamic_array(arr_dynamic, t);

	for (i = 0; i < t; i++) {
		for (j = 1; j <= arr_dynamic[i]; j++) {
			k = j;
			while (k != 0) {
				ans[k % 10]++;
				k /= 10;
			}
		}
		print_the_numbers(ans);
		printf("\n");
	}

	free(arr_dynamic);
	return 0;
}
