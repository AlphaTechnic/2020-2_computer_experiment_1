#include <stdio.h>
#include <stdlib.h>
#define MAX_STR_SIZE 1000 + 1 // ���ڿ��� �Էµ� �� �ִ� �ִ� ����(NULL���� ���)
#define MAX_ADD_SIZE 100 + 1  // ���ڿ��� �߰��� �� �ִ� �ִ� ����(NULL���� ���)

typedef struct node { // ���� ���Ḯ��Ʈ ����ü ����
	char value;
	struct node* next;
	struct node* prev;
}NODE;

// ������Ʈ���� �ʼ��� �䱸�� �Լ�
void left(NODE** cursor);
void right(NODE** cursor);
void del(NODE** cursor);
void add(NODE** cursor, char* data);
void quit(NODE* head);

// �߰��� ������ �Լ�
void init(NODE* head); // ���� ���Ḯ��Ʈ�� �ʱ�ȭ�ϴ� �Լ��̴�.
void print_dlist(NODE* head); // ���Ḯ��Ʈ ���� ���Ҹ� �ܼ� ȭ�鿡 ����ϴ� �Լ�. ������� ���� ���Ǿ���.
int my_strlen(char* str); // ���ڿ��� ���̸� �˾Ƴ��� �Լ�.
void clear_stdin(); // ���۸� ���� �Լ�.

void clear_stdin() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') {};
}

void main() {
	NODE* head = (NODE*)malloc(sizeof(NODE)); // ���߿��Ḯ��Ʈ ����
	init(head);                               // ���߿��Ḯ��Ʈ �ʱ�ȭ

	NODE* cursor = head;                      // ó���� cursor�� head�� ����Ű���� �Ѵ�. 
	
	char initial_str[MAX_STR_SIZE];  // ó�� �Է� �޴� ���ڿ��� �����ϴ� char �迭
	char add_str[MAX_STR_SIZE];      // A $ ������� �߰��� �ԷµǴ� ���ڿ��� �����ϴ� char �迭
	char command; // ��ɾ� ���ڸ� ����
	char for_getchar; // getchar()�� return���� �޴� ����. getchar()�� return���� �����ϰ� �ִٴ� �����޼��� ���Ÿ� ����.

	//printf("�Է� : ");
	if (scanf("%[^\n]s", initial_str) == -1) { // �ʱ� ���ڿ��� �Է¹޴´�. ����(����)���� �Է� �ޱ����� ó���� �Ͽ���.
		return;
	}
	for_getchar = getchar(); // ���๮�ڸ� ���ۿ��� ����.

	// main �Լ� ������ ������ ���� ���� ����Ʈ �ڷᱸ��(head)�� initial_str ���ڿ��� �����Ѵ�.
	// �Լ��� ������ �� �ڿ� cursor�� ������ �� �ڿ� ��ġ�ϰ� �ȴ�.
	add(&cursor, initial_str); 


	while (1) {
		//printf("�Է� : ");
		if (scanf("%c", &command) == -1) {
			return ;
		}
		switch (command) {
			case 'L':
				left(&cursor);
				break;
			case 'R':
				right(&cursor);
				break;
			case 'D':
				del(&cursor);
				break;
			case 'A': // �߰�(A) command�� ���, ����ڰ� �Է��� ���ڿ��� �Բ� �Է��Ѵ�.
				for_getchar = getchar(); // ���� ����
				if (scanf("%[^\n]s", add_str) == -1) { // ���ڿ��� �Է� �޴´�.
					return;
				}
				add(&cursor, add_str);
				break;
			case 'Q':
				quit(head);
				free(head); // head ���� �Ҵ� ����
				printf("(���α׷� ����)\n");
				return;
			default:            // �߸��� �Է��� �Ͽ��� ��, ���� ó��
				printf("wrong input\n\n");
				break;
		}
		clear_stdin();     // �Է¿� ���� �Լ� ������ ��ġ��, ���۸� ����ش�.
	}
}

void init(NODE* head) {
	head->value = 'Z'; // ���� �ҹ��ڸ��� �Է��ϴ� editor�̹Ƿ�, �빮�� Z�� head�� value������ �־, value�� ������ head�� ��¡�� �� �ְ� �Ѵ�.
	head->prev = head;
	head->next = head;
}

void print_dlist(NODE* head) {
	NODE* ptr;
	for (ptr = head->next; ptr != head; ptr = ptr->next) { // head�� ���� ������ ptr�� next��ũ�� Ÿ�� �̵��ϸ�, �ش� ����� value�� ����Ѵ�.
		printf("%c", ptr->value);
	}
	printf("\n");
}

void left(NODE** cursor) {
	// cursor�� head�� ����Ű�� �Ǹ� ���̻� cursor�� �̵��� �������� ����.
	// left�������� cursor�� ����ų �� �ִ� ���� �� �ں��� N��° ���, ..., 1��° ���, head �� (N + 1)�� �̴�.
	// cursor�� ����Ű�� ��尡 head�� ���� �����Ϳ��� cursor�� ù ��° ���� ���ʿ�(��, �� �տ�) �ִ� �Ͱ� ����.
	if ((*cursor)->value == 'Z') {
		return;
	}
	*cursor = (*cursor)->prev;
}

void right(NODE** cursor) {
	// cursor�� �� ��带 ����Ű�� �Ǹ� ���̻� cursor�� �̵��� �������� ����.
	// right�������� cursor�� ����ų �� �ִ� ���� �� �պ��� head, 1��° ���, ..., N��° ��� �� (N + 1)�� �̴�.
	// cursor�� ����Ű�� ��尡 ������ ����� ���� �����Ϳ��� cursor�� ������ ���� �����ʿ�(��, �� �ڿ�) �ִ� �Ͱ� ����.
	if ((*cursor)->next->value == 'Z') { 
		return;
	}
	*cursor = (*cursor)->next;
}

void del(NODE** cursor) {
	NODE* removed = (*cursor);

	// ���� ���Ḯ��Ʈ�� ����ִٸ�,(editor�� �ƹ��� ���ڵ� ��ϵǾ� ���� �ʴٸ�) del �Լ��� �ƹ��� ���۵� �������� �ʴ´�.
	if (removed->value == 'Z') { 
		return;
	}
	removed->prev->next = removed->next;  // 'cursor�� ����Ű�� ����� pre���'�� 'cursor�� ����Ű�� ����� next ���'�� next�� ����Ű�� �Ѵ�.
	removed->next->prev = removed->prev;  // 'cursor�� ����Ű�� ����� next���'�� 'curor�� ����Ű�� ����� pre ���'�� prev�� ����Ű�� �Ѵ�.
	*cursor = removed->prev;          // 'cursor�� ����Ű�� ���'�� 'removed�� pre���'�� �ǵ��� ����.
	free(removed);
}

void add(NODE** cursor, char* data) {
	for (int i = 0; i < my_strlen(data); i++) { // data ���ڿ��� ���Ҹ� �տ������� �湮�ϸ鼭, �̸� ���� ���Ḯ��Ʈ �ڷᱸ���� �߰��Ѵ�.
		NODE* ptr = (*cursor)->next;
		if (ptr == NULL) {
			printf("alloc err!\n");
			exit(1);
		}
		NODE* new_node = (NODE*)malloc(sizeof(NODE));
		if (new_node == NULL) {
			printf("alloc err!\n");
			exit(1);
		}
		
		// cursor�� ����Ű�� ����� �����ʿ� ���ο� ��带 �����Ѵ�.
		// new_node�� �����Ѵ�. new_node�� "cursor�� ����Ű�� ���"�� "cursor�� ����Ű�� ����� next ���" ���̿� ���� �ȴ�.
		new_node->value = data[i];
		new_node->prev = (*cursor); // new_node�� 'cursor�� ����Ű�� ���'�� prev�� ����Ű�� �Ѵ�.
		new_node->next = ptr;       // new_node�� 'cursor�� ����Ű�� ����� ���� ���'�� next�� ����Ű�� �Ѵ�.

		(*cursor)->next = new_node;  // 'cursor�� ����Ű�� ���'�� next�� new_node�� ����Ű�� �Ѵ�.
		ptr->prev = new_node;        // 'cursor�� ����Ű�� ����� ���� ���'�� prev�� new_node�� ����Ű�� �Ѵ�.
		(*cursor) = new_node;	     // 'cursor�� ����Ű�� ���'�� new_node�� ����.
	}
}

void quit(NODE* head) {
	FILE* fp;
	if ((fp = fopen("data.txt", "w")) == NULL) { // ������� ������ ����.
		printf("file open err!\n");
		return;
	}

	NODE* ptr;
	// ptr�� next��ũ�� Ÿ�� �̵��ϸ�, head�� ���� ������ �ش� ����� value�� data.txt ���Ͽ� ����Ѵ�.
	for (ptr = head->next; ptr != head; ptr = ptr->next) { 
		fprintf(fp, "%c", ptr->value);
	}
	fclose(fp);
}

int my_strlen(char* str) { // ���ڷ� �Է¹��� ���ڿ��� ���̸� ����ϴ� �Լ�.
	int i = 0;
	while (str[i]) {
		i++;
	}
	return i;
}
