#include "Header.h" 

int main() {
	int t;   // testcase의 갯수를 받는 변수
	int i, j, k;        // for문을 컨트롤하는 변수  
	int check = 1;   // while문 진입여부를 컨트롤하는 변수
	int ans[10] = { 0, };   // 최종 계산 결과가 입력되는 배열 
	int* arr_dynamic;

	// 사용자로부터 testcase의 갯수 t를 입력받는다. 
	printf("please input number of Test Case : ");
	scanf("%d", &t);
	get_num_of_TestCase(&t);

	// 입력으로 들어오는 책의 페이지 수를 arr_dynamic에 저장
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
