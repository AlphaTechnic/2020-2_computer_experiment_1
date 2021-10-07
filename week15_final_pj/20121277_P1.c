#include <stdio.h>
#include <stdlib.h>

// 프로젝트에서 필수로 요구된 함수
int empty(int* vector);
void push(int** vector, int* element_size, int* alloc_size, int value);
int pop(int** vector, int* element_size, int* alloc_size);
int front(int* vector, int element_size);
int back(int* vector, int element_size);
int print_all(int* vector, int element_size);

// 추가로 구현한 함수
void init(int** vector, int* element_size, int* alloc_size); // vector와 element_size, alloc_size를 초기화한다.
int full(int* element_size, int* alloc_size); // push() 연산을 가정했을 때, 주어진 vector가 포화상태가 되는지 점검한다.	
int waste(int* element_size, int* alloc_size); // pop() 연산을 가정했을 때, 메모리의 낭비가 없게 되는지 점검한다.
void clear_stdin(); // 버퍼를 비우는 함수

void clear_stdin() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') {};
}

void main() {
	int* vector;
	int element_size = 0;
	int alloc_size = 0;
	
	int command;
	int for_getchar = 1;
	int push_num;
	int tmp;

	init(&vector, &element_size, &alloc_size); // 수행 결과 : vector[0] == 0, element_size == 0, alloc_size == 1

	while (1) {
		//printf("입력 : ");
		if (scanf("%d", &command) == -1) { // command 입력 받음
			return;
		}

		//printf("출력 : ");
		switch (command) {
		case 0:
			printf("%d\n", empty(vector)); // empty
			break;
		case 1:
			for_getchar = getchar(); // 공백 제거
			if (scanf("%d", &push_num) == -1) { // push 커맨드에서는 push_num을 함께 입력 받는다.
				return;
			}
			push(&vector, &element_size, &alloc_size, push_num); // push
			printf("%d\n", alloc_size);
			break;
		case 2:
			if (empty(vector)) { // vector가 공백이라면,
				printf("-1\n");   // -1을 출력
				break;
			}
			tmp = pop(&vector, &element_size, &alloc_size); // vector가 공백이 아니면, 
			printf("%d %d\n", tmp, alloc_size);             // pop된 원소를 출력
			break;
		case 3:
			printf("%d\n", front(vector, element_size)); // front
			break;
		case 4:
			printf("%d\n", back(vector, element_size));  // back
			break;
		case 5:
			print_all(vector, element_size);  // print_all
			break;
		default:                                // 잘못된 커맨드를 처리한다.
			printf("wrong input!\n");
		}
		//printf("\n");
		clear_stdin();                        // 입력 하나가 수행되면, 버퍼를 비우고 새로운 입력을 받을 수 있는 상태로 만든다.
	}
	return ;
}

// 초기화
void init(int** vector, int* element_size, int* alloc_size) {
	*vector = (int*)malloc(1 * sizeof(int));
	if ((*vector) == NULL) {
		printf("alloc fail\n");
		exit(1);
	}
	(*vector)[0] = 0; // vector[0] == 0으로 초기화
	(*alloc_size)++; // alloc_size == 1로 초기화
	return;
}

// 공백 검사
int empty(int* vector) {
	return vector[0] == 0;
}

// 포화상태 검사
int full(int* element_size, int* alloc_size) {
	if ((*element_size + 1) >= (*alloc_size)) { // element_size를 1 증가 시켰을 때(push의 상황을 가정), alloc_size와 같아진다면,
		return 1;
	}
	else {
		return 0;
	}
}

// 메모리의 낭비가 있는지 검사
int waste(int* element_size, int* alloc_size) {
	// element 개수가 0개인 경우 예외처리. pop() 연산에서 waste() 함수를 쓰기 전에,
	// vector가 비었을 경우에 대한 예외처리를 진행하기 때문에, 이 조건문 내부로 들어갈 일은 없을 것이다.
	// 디버깅을 위한 조건문.
	if ((*element_size) == 0) { 
		printf("incorrect use of function");
		exit(1);
	}

	// alloc_size를 절반으로 줄이고도, element_size - 1 (-1 의 의미는 pop을 진행했다는 가정) 보다 크다.
	if ((*element_size) - 1 < (*alloc_size) / 2) {
		return 1;
	}
	else return 0;
}

// 원소 삽입
void push(int** vector, int* element_size, int* alloc_size, int value) {
	int* ptr = *vector;

	if (full(element_size, alloc_size)) { // 메모리가 꽉 찼다면,
		(*alloc_size) *= 2;               // alloc_size를 2배 키우고,
		ptr = (int*)realloc(*vector, (*alloc_size) * sizeof(int)); // 2배 키운 alloc_size만큼 realloc을 진행한다.
		if ((ptr) == NULL) {
			printf("alloc fail\n");
			exit(1);
		}
	}
	*vector = ptr;
	(*vector)[++(*element_size)] = value; // element_size를 1 키우고, 그 index자리에 value값을 할당한다.
	(*vector)[0] = (*element_size);        // element_size를 담고있는, vector[0]의 값을 업데이트
}

// 원소 삭제
int pop(int** vector, int* element_size, int* alloc_size) {
	int tmp = (*vector)[*element_size];
	int* ptr = *vector;

	if (empty(*vector)) {
		return -1;
	}
	if (waste(element_size, alloc_size)) { // 메모리의 낭비가 생긴다면, 
		(*alloc_size) /= 2;                  // alloc_size를 절반으로 줄이고, 
		ptr = (int*)realloc(*vector, (*alloc_size) * sizeof(int)); // 절반으로 줄인 alloc_size만큼 realloc을 진행한다.
		if ((ptr) == NULL) {
			printf("alloc fail\n");
			exit(1);
		}
	}
	*vector = ptr;
	(*vector)[0] = --(*element_size); // element_size를 1 줄이고, 그 값으로 vector[0]을 업데이트한다.
	return tmp; // 삭제한 원소를 pop
}

// vector의 가장 앞 원소를 보여준다.
int front(int* vector, int element_size) {
	if (empty(vector)) { // vector가 비어있는 경우 예외처리
		return -1;
	}
	return vector[1]; // vector의 원소는 1번 인덱스부터 시작한다.
}

// vector의 가장 뒤 원소를 보여준다.
int back(int* vector, int element_size) {
	if (empty(vector)) {
		return -1;
	}
	return vector[element_size];
}

// vector 자료구조의 원소들을 알려준다.
int print_all(int* vector, int element_size) {
	if (empty(vector)) { // vector가 비어있는 경우 -1을 출력
		printf("-1\n");
		return -1;
	}

	for (int i = 1; i <= element_size; i++) { // vector의 1번 인덱스부터 끝까지의 원소를 출력한다.
		printf("%d ", vector[i]);
	}
	printf("\n");
	return 1;
}

