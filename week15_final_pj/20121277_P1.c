#include <stdio.h>
#include <stdlib.h>

// ������Ʈ���� �ʼ��� �䱸�� �Լ�
int empty(int* vector);
void push(int** vector, int* element_size, int* alloc_size, int value);
int pop(int** vector, int* element_size, int* alloc_size);
int front(int* vector, int element_size);
int back(int* vector, int element_size);
int print_all(int* vector, int element_size);

// �߰��� ������ �Լ�
void init(int** vector, int* element_size, int* alloc_size); // vector�� element_size, alloc_size�� �ʱ�ȭ�Ѵ�.
int full(int* element_size, int* alloc_size); // push() ������ �������� ��, �־��� vector�� ��ȭ���°� �Ǵ��� �����Ѵ�.	
int waste(int* element_size, int* alloc_size); // pop() ������ �������� ��, �޸��� ���� ���� �Ǵ��� �����Ѵ�.
void clear_stdin(); // ���۸� ���� �Լ�

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

	init(&vector, &element_size, &alloc_size); // ���� ��� : vector[0] == 0, element_size == 0, alloc_size == 1

	while (1) {
		//printf("�Է� : ");
		if (scanf("%d", &command) == -1) { // command �Է� ����
			return;
		}

		//printf("��� : ");
		switch (command) {
		case 0:
			printf("%d\n", empty(vector)); // empty
			break;
		case 1:
			for_getchar = getchar(); // ���� ����
			if (scanf("%d", &push_num) == -1) { // push Ŀ�ǵ忡���� push_num�� �Բ� �Է� �޴´�.
				return;
			}
			push(&vector, &element_size, &alloc_size, push_num); // push
			printf("%d\n", alloc_size);
			break;
		case 2:
			if (empty(vector)) { // vector�� �����̶��,
				printf("-1\n");   // -1�� ���
				break;
			}
			tmp = pop(&vector, &element_size, &alloc_size); // vector�� ������ �ƴϸ�, 
			printf("%d %d\n", tmp, alloc_size);             // pop�� ���Ҹ� ���
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
		default:                                // �߸��� Ŀ�ǵ带 ó���Ѵ�.
			printf("wrong input!\n");
		}
		//printf("\n");
		clear_stdin();                        // �Է� �ϳ��� ����Ǹ�, ���۸� ���� ���ο� �Է��� ���� �� �ִ� ���·� �����.
	}
	return ;
}

// �ʱ�ȭ
void init(int** vector, int* element_size, int* alloc_size) {
	*vector = (int*)malloc(1 * sizeof(int));
	if ((*vector) == NULL) {
		printf("alloc fail\n");
		exit(1);
	}
	(*vector)[0] = 0; // vector[0] == 0���� �ʱ�ȭ
	(*alloc_size)++; // alloc_size == 1�� �ʱ�ȭ
	return;
}

// ���� �˻�
int empty(int* vector) {
	return vector[0] == 0;
}

// ��ȭ���� �˻�
int full(int* element_size, int* alloc_size) {
	if ((*element_size + 1) >= (*alloc_size)) { // element_size�� 1 ���� ������ ��(push�� ��Ȳ�� ����), alloc_size�� �������ٸ�,
		return 1;
	}
	else {
		return 0;
	}
}

// �޸��� ���� �ִ��� �˻�
int waste(int* element_size, int* alloc_size) {
	// element ������ 0���� ��� ����ó��. pop() ���꿡�� waste() �Լ��� ���� ����,
	// vector�� ����� ��쿡 ���� ����ó���� �����ϱ� ������, �� ���ǹ� ���η� �� ���� ���� ���̴�.
	// ������� ���� ���ǹ�.
	if ((*element_size) == 0) { 
		printf("incorrect use of function");
		exit(1);
	}

	// alloc_size�� �������� ���̰�, element_size - 1 (-1 �� �ǹ̴� pop�� �����ߴٴ� ����) ���� ũ��.
	if ((*element_size) - 1 < (*alloc_size) / 2) {
		return 1;
	}
	else return 0;
}

// ���� ����
void push(int** vector, int* element_size, int* alloc_size, int value) {
	int* ptr = *vector;

	if (full(element_size, alloc_size)) { // �޸𸮰� �� á�ٸ�,
		(*alloc_size) *= 2;               // alloc_size�� 2�� Ű���,
		ptr = (int*)realloc(*vector, (*alloc_size) * sizeof(int)); // 2�� Ű�� alloc_size��ŭ realloc�� �����Ѵ�.
		if ((ptr) == NULL) {
			printf("alloc fail\n");
			exit(1);
		}
	}
	*vector = ptr;
	(*vector)[++(*element_size)] = value; // element_size�� 1 Ű���, �� index�ڸ��� value���� �Ҵ��Ѵ�.
	(*vector)[0] = (*element_size);        // element_size�� ����ִ�, vector[0]�� ���� ������Ʈ
}

// ���� ����
int pop(int** vector, int* element_size, int* alloc_size) {
	int tmp = (*vector)[*element_size];
	int* ptr = *vector;

	if (empty(*vector)) {
		return -1;
	}
	if (waste(element_size, alloc_size)) { // �޸��� ���� ����ٸ�, 
		(*alloc_size) /= 2;                  // alloc_size�� �������� ���̰�, 
		ptr = (int*)realloc(*vector, (*alloc_size) * sizeof(int)); // �������� ���� alloc_size��ŭ realloc�� �����Ѵ�.
		if ((ptr) == NULL) {
			printf("alloc fail\n");
			exit(1);
		}
	}
	*vector = ptr;
	(*vector)[0] = --(*element_size); // element_size�� 1 ���̰�, �� ������ vector[0]�� ������Ʈ�Ѵ�.
	return tmp; // ������ ���Ҹ� pop
}

// vector�� ���� �� ���Ҹ� �����ش�.
int front(int* vector, int element_size) {
	if (empty(vector)) { // vector�� ����ִ� ��� ����ó��
		return -1;
	}
	return vector[1]; // vector�� ���Ҵ� 1�� �ε������� �����Ѵ�.
}

// vector�� ���� �� ���Ҹ� �����ش�.
int back(int* vector, int element_size) {
	if (empty(vector)) {
		return -1;
	}
	return vector[element_size];
}

// vector �ڷᱸ���� ���ҵ��� �˷��ش�.
int print_all(int* vector, int element_size) {
	if (empty(vector)) { // vector�� ����ִ� ��� -1�� ���
		printf("-1\n");
		return -1;
	}

	for (int i = 1; i <= element_size; i++) { // vector�� 1�� �ε������� �������� ���Ҹ� ����Ѵ�.
		printf("%d ", vector[i]);
	}
	printf("\n");
	return 1;
}

