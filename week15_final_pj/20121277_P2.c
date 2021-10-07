#include <stdio.h>
#include <stdlib.h>
#define MAX_STR_SIZE 1000 + 1 // 문자열이 입력될 수 있는 최대 길이(NULL문자 고려)
#define MAX_ADD_SIZE 100 + 1  // 문자열을 추가할 수 있는 최대 길이(NULL문자 고려)

typedef struct node { // 이중 연결리스트 구조체 선언
	char value;
	struct node* next;
	struct node* prev;
}NODE;

// 프로젝트에서 필수로 요구된 함수
void left(NODE** cursor);
void right(NODE** cursor);
void del(NODE** cursor);
void add(NODE** cursor, char* data);
void quit(NODE* head);

// 추가로 구현한 함수
void init(NODE* head); // 이중 연결리스트를 초기화하는 함수이다.
void print_dlist(NODE* head); // 연결리스트 내의 원소를 콘솔 화면에 출력하는 함수. 디버깅을 위해 사용되었다.
int my_strlen(char* str); // 문자열의 길이를 알아내는 함수.
void clear_stdin(); // 버퍼를 비우는 함수.

void clear_stdin() {
	int ch;
	while ((ch = getchar()) != EOF && ch != '\n') {};
}

void main() {
	NODE* head = (NODE*)malloc(sizeof(NODE)); // 이중연결리스트 선언
	init(head);                               // 이중연결리스트 초기화

	NODE* cursor = head;                      // 처음에 cursor가 head를 가리키도록 한다. 
	
	char initial_str[MAX_STR_SIZE];  // 처음 입력 받는 문자열을 저장하는 char 배열
	char add_str[MAX_STR_SIZE];      // A $ 명령으로 추가로 입력되는 문자열을 저장하는 char 배열
	char command; // 명령어 문자를 저장
	char for_getchar; // getchar()의 return값을 받는 변수. getchar()의 return값을 무시하고 있다는 오류메세지 제거를 위함.

	//printf("입력 : ");
	if (scanf("%[^\n]s", initial_str) == -1) { // 초기 문자열을 입력받는다. 공백(띄어쓰기)까지 입력 받기위한 처리를 하였다.
		return;
	}
	for_getchar = getchar(); // 개행문자를 버퍼에서 비운다.

	// main 함수 내에서 선언한 이중 연결 리스트 자료구조(head)에 initial_str 문자열을 저장한다.
	// 함수의 수행을 한 뒤에 cursor는 문장의 맨 뒤에 위치하게 된다.
	add(&cursor, initial_str); 


	while (1) {
		//printf("입력 : ");
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
			case 'A': // 추가(A) command의 경우, 사용자가 입력할 문자열을 함께 입력한다.
				for_getchar = getchar(); // 띄어쓰기 제거
				if (scanf("%[^\n]s", add_str) == -1) { // 문자열을 입력 받는다.
					return;
				}
				add(&cursor, add_str);
				break;
			case 'Q':
				quit(head);
				free(head); // head 동적 할당 해제
				printf("(프로그램 종료)\n");
				return;
			default:            // 잘못된 입력을 하였을 때, 예외 처리
				printf("wrong input\n\n");
				break;
		}
		clear_stdin();     // 입력에 따른 함수 수행을 마치면, 버퍼를 비워준다.
	}
}

void init(NODE* head) {
	head->value = 'Z'; // 오직 소문자만을 입력하는 editor이므로, 대문자 Z를 head의 value값으로 주어서, value값 만으로 head를 상징할 수 있게 한다.
	head->prev = head;
	head->next = head;
}

void print_dlist(NODE* head) {
	NODE* ptr;
	for (ptr = head->next; ptr != head; ptr = ptr->next) { // head의 다음 노드부터 ptr이 next링크를 타고 이동하며, 해당 노드의 value를 출력한다.
		printf("%c", ptr->value);
	}
	printf("\n");
}

void left(NODE** cursor) {
	// cursor가 head를 가리키게 되면 더이상 cursor의 이동을 진행하지 않음.
	// left연산으로 cursor가 가리킬 수 있는 노드는 맨 뒤부터 N번째 노드, ..., 1번째 노드, head 총 (N + 1)개 이다.
	// cursor가 가리키는 노드가 head인 것은 에디터에서 cursor가 첫 번째 문자 왼쪽에(즉, 맨 앞에) 있는 것과 같다.
	if ((*cursor)->value == 'Z') {
		return;
	}
	*cursor = (*cursor)->prev;
}

void right(NODE** cursor) {
	// cursor가 끝 노드를 가리키게 되면 더이상 cursor의 이동을 진행하지 않음.
	// right연산으로 cursor가 가리킬 수 있는 노드는 맨 앞부터 head, 1번째 노드, ..., N번째 노드 총 (N + 1)개 이다.
	// cursor가 가리키는 노드가 마지막 노드인 것은 에디터에서 cursor가 마지막 문자 오른쪽에(즉, 맨 뒤에) 있는 것과 같다.
	if ((*cursor)->next->value == 'Z') { 
		return;
	}
	*cursor = (*cursor)->next;
}

void del(NODE** cursor) {
	NODE* removed = (*cursor);

	// 이중 연결리스트가 비어있다면,(editor에 아무런 문자도 기록되어 있지 않다면) del 함수는 아무런 동작도 수행하지 않는다.
	if (removed->value == 'Z') { 
		return;
	}
	removed->prev->next = removed->next;  // 'cursor가 가리키는 노드의 pre노드'가 'cursor가 가리키는 노드의 next 노드'를 next로 가리키게 한다.
	removed->next->prev = removed->prev;  // 'cursor가 가리키는 노드의 next노드'가 'curor가 가리키는 노드의 pre 노드'를 prev로 가리키게 한다.
	*cursor = removed->prev;          // 'cursor가 가리키는 노드'가 'removed의 pre노드'가 되도록 조정.
	free(removed);
}

void add(NODE** cursor, char* data) {
	for (int i = 0; i < my_strlen(data); i++) { // data 문자열의 원소를 앞에서부터 방문하면서, 이를 이중 연결리스트 자료구조에 추가한다.
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
		
		// cursor가 가리키는 노드의 오른쪽에 새로운 노드를 삽입한다.
		// new_node를 생성한다. new_node는 "cursor가 가리키는 노드"와 "cursor가 가리키는 노드의 next 노드" 사이에 들어가게 된다.
		new_node->value = data[i];
		new_node->prev = (*cursor); // new_node가 'cursor가 가리키는 노드'를 prev로 가리키게 한다.
		new_node->next = ptr;       // new_node가 'cursor가 가리키는 노드의 다음 노드'를 next로 가리키게 한다.

		(*cursor)->next = new_node;  // 'cursor가 가리키는 노드'가 next로 new_node를 가리키게 한다.
		ptr->prev = new_node;        // 'cursor가 가리키는 노드의 다음 노드'가 prev로 new_node를 가리키게 한다.
		(*cursor) = new_node;	     // 'cursor가 가리키는 노드'를 new_node로 조정.
	}
}

void quit(NODE* head) {
	FILE* fp;
	if ((fp = fopen("data.txt", "w")) == NULL) { // 쓰기모드로 파일을 연다.
		printf("file open err!\n");
		return;
	}

	NODE* ptr;
	// ptr이 next링크를 타고 이동하며, head의 다음 노드부터 해당 노드의 value를 data.txt 파일에 출력한다.
	for (ptr = head->next; ptr != head; ptr = ptr->next) { 
		fprintf(fp, "%c", ptr->value);
	}
	fclose(fp);
}

int my_strlen(char* str) { // 인자로 입력받은 문자열의 길이를 출력하는 함수.
	int i = 0;
	while (str[i]) {
		i++;
	}
	return i;
}
