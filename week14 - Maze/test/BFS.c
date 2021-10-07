#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define true 1
#define false 0

// 미로 그리는 배율(size 조정). 1이 기준. 줄이고 싶다면 ㅡ 0~1 / 늘리고 싶다면 - 1 이상
float magnification = 0.5;
// (translation, translation) 만큼 미로를 평행이동한다. ex ) 1 할당 -> (1,1) 만큼 미로를 평행이동.
float translation = 1;
// 미로 벽의 굵기
int thickness = 2;

float tune(float num) {
	return magnification * num + translation;
}

typedef struct vertex { // vertex가 가지고 있는 자료구조
	int x; int y;
	int r_flag; int d_flag;
	int wrong_visited;
	struct vertex* before;
}vertex;

typedef struct QueueNode { // QueueNode
	struct vertex* vertex_ptr;
	struct QueueNode* link; // DFS 탐색시 탐색했던 경로 저장을 위해 사용하는 변수.
	int have_edge;
}QueueNode;

typedef struct LinkedQueueList { // 연결리스트로 구현한 스택을 선언
	struct QueueNode* front, * rear;
}LinkedQueueList;

FILE* fp;
const char* filename;
int file_flag = false;
vertex** maze = NULL; // 이차원 배열 선언

int HEIGHT, WIDTH;
int draw_flag = false;

int readFile();

void initQ(LinkedQueueList* q);
int isQ_empty(LinkedQueueList* q);
void enqueue(LinkedQueueList* q, vertex* vertex_ptr);
vertex* dequeue(LinkedQueueList* q);

void freeMemory();
///////////////////////////////2주차 실습(DFS)///////////////////////////////////////

int dfs_flag = false;

typedef struct StackNode { // vertex_ptr을 data로 가지고 있는 StackNode 구조체
	struct vertex* vertex_ptr;
	struct StackNode* link;
	int junction; // dfs 탐색시 최단경로가 아니라고 판정이 되면, edge를 끊는다.
}StackNode;

typedef struct LinkedStackList { // 연결리스트로 구현한 스택을 선언
	struct StackNode* front, * rear;
}LinkedStackList;

LinkedStackList Shortest_Path_by_DFS; // 최단 경로의 트리를 저장
LinkedStackList Visited_Path_by_DFS; // 트리의 방문 노드 중 최단 경로를 제외한 부분트리 저장

void initS(LinkedStackList* s);
void push(LinkedStackList* s, vertex* vertex_ptr);
vertex* pop(LinkedStackList* s);
int isS_empty(LinkedStackList* s);
StackNode* peek(LinkedStackList* s);
void DFS();
//////////////////////////////////2주차 과제(BFS)/////////////////////////////////

int bfs_flag = false;
LinkedQueueList Queue_for_vertex_arr;
LinkedQueueList Queue_for_BFS;
LinkedStackList B_visit_root;
void BFS();


void initQ(LinkedQueueList* q) {
	q->front = q->rear = NULL;
}
void initS(LinkedStackList* s) {
	s->front = s->rear = NULL;
}

int readFile() {
	char ch;
	int floor;
	vertex* current_line; // 1차원 배열
	vertex* tmp_vertices_arr; // 1차원 배열


	if ((fp = fopen("PerfectMaze.maz", "r")) == NULL) {
		printf("file open err!");
		return false;
	}

	HEIGHT = 0;
	WIDTH = 0;

	if (fscanf(fp, "%c", &ch) == -1) { // '+' 버림
		printf("scanf err!");
		return false;
	}
	while (1) { // 미로의 천장을 읽으면서 WIDTH를 알아냄.		
		// '-' 읽음
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}

		if (ch == 10) { // 개행문자 만나면 탈출 (10 means "\n")
			break;
		}

		// '+' 읽음
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}
		WIDTH++;
	}

	current_line = NULL;
	tmp_vertices_arr = NULL;
	current_line = (vertex*)malloc(sizeof(vertex) * WIDTH);
	if (current_line == NULL) {
		printf("alloc err!\n");
		return false;
	}
	enqueue(&Queue_for_vertex_arr, current_line); // Queue에 삽입

	while (1) { // 2가지 작업 :  1. cell을 읽는다. 2. 바닥을 읽는다.
		floor = 1; // 미로의 맨 밑바닥임을 표시하는 변수. 뚫린 바닥을 만나면 0이 할당되도록 설계.
		if (fscanf(fp, "%c", &ch) == -1) { // '|' 버림.
			printf("scanf err!");
			return false;
		}

		// 1. cell들을 읽는다.
		for (int i = 0; i < WIDTH; i++) {
			current_line[i].x = i;
			current_line[i].y = HEIGHT;

			// ' ' 읽는다.
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}

			// '|'(124)(오른쪽 벽 있음) 또는 ' '(32)(오른쪽 벽 없음)가 들어온다.
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}
			if (ch == 32) { // ' ' 라면, 
				current_line[i].r_flag = 1;
			}
			else if (ch == 124) { // '|' 이라면,
				current_line[i].r_flag = 0;
			}
		}
		current_line[WIDTH - 1].r_flag = 0;

		// 마지막 '\n'을 버림.
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}

		///////////////////////////////////////////////////////
		// 2. 바닥을 읽는다. 맨 처음 등장하는 '+'를 버림.
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}

		// '-'(45)(바닥 막힘) 또는 ' '(32)(바닥 뚫림)가 들어온다. ('+'는 버린다.)
		tmp_vertices_arr = (vertex*)malloc(sizeof(vertex) * WIDTH);
		if (tmp_vertices_arr == NULL) {
			printf("alloc err!\n");
			exit(1);
		}
		enqueue(&Queue_for_vertex_arr, tmp_vertices_arr);
		HEIGHT++;

		for (int i = 0; i < WIDTH; i++) {
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}
			if (ch == 32) { // 아래쪽 뚫려있는데,(32 means 'space')
				if (floor) { // '바닥'이라고 되어있으면,
					floor = 0; // '바닥이 아니다'라고 floor 변수를 조정
				}
				current_line[i].d_flag = 1;
			}
			else if (ch == 45) {
				current_line[i].d_flag = 0;
			}
			// '+'를 버린다.
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}
		}

		if (floor) { // 미로의 맨 밑바닥에 왔다면,
			for (int i = 0; i < WIDTH; i++) {
				current_line[i].d_flag = 0;
			}
			break;
		}

		current_line = tmp_vertices_arr;
		tmp_vertices_arr = NULL;

		// 마지막 끝에 개행문자 '\n'을 버린다.
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}
	}
	//HEIGHT++;

	file_flag = true;

	current_line = NULL;
	tmp_vertices_arr = NULL;
	(&Queue_for_vertex_arr)->rear = NULL;
	fclose(fp);

	// Queue에 담긴 cell 정보를 maze 2차원 배열에 저장.
	maze = (vertex**)malloc(sizeof(vertex*) * HEIGHT);
	if (maze == NULL) {
		printf("alloc err!");
		exit(1);
	}
	for (int i = 0; i < HEIGHT; i++) {
		maze[i] = dequeue(&Queue_for_vertex_arr);
	}

	/* for test */
	//printf("(rows : %d, cols %d)\n", HEIGHT, WIDTH);
	//for (int i = 0; i < HEIGHT; i++) {
	//	for (int j = 0; j < WIDTH; j++) {
	//		printf("[x: %d, y:%d](right:%d, down:%d) ", maze[i][j].x, maze[i][j].y ,maze[i][j].r_flag, maze[i][j].d_flag);
	//	}
	//	printf("\n");
	//}

	return true;
}

////////////////Queue ADT///////////////////////
int isQ_empty(LinkedQueueList* q) {
	return (q->front == NULL);
}

void enqueue(LinkedQueueList* q, vertex* vertex_ptr) {
	QueueNode* tmp = (QueueNode*)malloc(sizeof(QueueNode));
	if (tmp == NULL) {
		printf("alloc err!\n");
		exit(1);
	}
	tmp->vertex_ptr = vertex_ptr;
	tmp->link = NULL;

	if (isQ_empty(q)) {
		q->front = tmp;
		q->rear = tmp;
	}
	else {
		q->rear->link = tmp;
		q->rear = tmp;
	}
	return;
}

vertex* dequeue(LinkedQueueList* q) {
	QueueNode* tmp = q->front;
	vertex* vertex_ptr;

	if (isQ_empty(q)) {
		printf("Queue is empty!\n");
		exit(1);
	}
	else {
		vertex_ptr = tmp->vertex_ptr;
		q->front = q->front->link;
		if (isQ_empty(q)) {
			q->rear = NULL;
		}
		free(tmp);

		return vertex_ptr;
	}
}

void freeMemory() {
	// maze 이차원 배열 비움.
	if (maze != NULL) {
		for (int i = 0; i < HEIGHT; i++) {
			free(maze[i]);
		}
		free(maze);
		maze = NULL;
	}

	// Queue를 비움.
	if (file_flag) {
		while (true) {
			if ((&Queue_for_vertex_arr)->front == NULL) break;
			dequeue(&Queue_for_vertex_arr);
		}
	}
	if (isQ_empty(&Queue_for_vertex_arr)) {
		QueueNode* ptr = (&Queue_for_vertex_arr)->front;
		(&Queue_for_vertex_arr)->rear = NULL;
		while (ptr != NULL) {
			free((&Queue_for_vertex_arr)->front->vertex_ptr);
			(&Queue_for_vertex_arr)->front = ptr->link;
			free(ptr);
			ptr = (&Queue_for_vertex_arr)->front;
		}
	}
}

//////////////////////// 스택 ADT //////////////////////////
int isS_empty(LinkedStackList* s) {
	return (s->front == NULL);
}

vertex* pop(LinkedStackList* s) {
	StackNode* tmp = s->front;
	vertex* V;

	if (isS_empty(s)) {
		printf("Stack is empty!\n");
		exit(1);
	}
	else {
		V = s->front->vertex_ptr;
		s->front = s->front->link;
		if (isS_empty(s)) {
			s->rear = NULL;
		}
		free(tmp);
		return V;
	}
}

void push(LinkedStackList* s, vertex* vertex_ptr) {
	StackNode* tmp = (StackNode*)malloc(sizeof(StackNode));
	if (tmp == NULL) {
		printf("alloc err!\n");
		exit(1);
	}
	tmp->vertex_ptr = vertex_ptr;
	tmp->junction = true;
	tmp->link = NULL;

	if (isS_empty(s)) {
		s->front = tmp;
		s->rear = tmp;
	}
	else {
		tmp->link = s->front;
		s->front = tmp;
	}
	return;
}

StackNode* peek(LinkedStackList* s) {
	if (isS_empty(s)) {
		printf("stack is empty!\n");
		exit(1);
	}
	return s->front;
}



void DFS() {
	vertex* v;

	int** visited;
	visited = (int**)malloc(sizeof(int*) * HEIGHT);
	for (int i = 0; i < HEIGHT; i++) {
		visited[i] = (int*)malloc(sizeof(int) * WIDTH);
	}
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			visited[i][j] = false;
		}
	}

	int** wrong_visited; // 분기점 ~ LeafNode까지를 표시 (잘못 들른 path)
	wrong_visited = (int**)malloc(HEIGHT * sizeof(int*));
	for (int i = 0; i < HEIGHT; i++) {
		wrong_visited[i] = (int*)malloc(sizeof(int) * WIDTH);
	}
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			wrong_visited[i][j] = false;
		}
	}

	int flag = false;
	v = &maze[0][0];

	initS(&Visited_Path_by_DFS);
	initS(&Shortest_Path_by_DFS);
	StackNode* tmp;

	while (1) {
		visited[v->y][v->x] = true; // mark v as visited;
		if ((v->y == HEIGHT - 1) && (v->x == WIDTH - 1)) { // 종료 조건 : 목적지 도달
			push(&Shortest_Path_by_DFS, v);
			break;
		}
//		if (!wrong_visited[v->y][v->x]) {
			// 오른쪽
			if ((v->r_flag) && !visited[v->y][v->x + 1]) { // if ( S.top has an unvisited adjacent node )
				if (flag) {
					push(&Visited_Path_by_DFS, v);     // Visited_Path에 push();
					tmp = peek(&Visited_Path_by_DFS);
					tmp->junction = true; // 분기점에서 edge를 끊어낸다.
				}
				push(&Shortest_Path_by_DFS, v);    // Shortest_Path에 push();
				v = &maze[v->y][v->x + 1];
				flag = false;
				continue;
			}
			// 아래
			if ((v->d_flag) && !visited[v->y + 1][v->x]) { // if ( S.top has an unvisited adjacent node )
				if (flag) {
					push(&Visited_Path_by_DFS, v);  // Visited_Path에 push();
					tmp = peek(&Visited_Path_by_DFS);
					tmp->junction = true;  // 분기점에서 edge를 끊어낸다.
				}
				push(&Shortest_Path_by_DFS, v);  // Shortest_Path에 push();
				v = &maze[v->y + 1][v->x];
				flag = false;
				continue;
			}
			// 왼쪽
			if (v->x > 0) {
				if (maze[v->y][v->x - 1].r_flag && !visited[v->y][v->x - 1]) { // if ( S.top has an unvisited adjacent node )
					if (flag) {
						push(&Visited_Path_by_DFS, v);    // Visited_Path에 push();
						tmp = peek(&Visited_Path_by_DFS);
						tmp->junction = true;  // 분기점에서 edge를 끊어낸다.
					}
					push(&Shortest_Path_by_DFS, v);        // Shortest_Path에 push();
					v = &maze[v->y][v->x - 1];
					flag = false;
					continue;
				}
			}
			// 위
			if (v->y > 0) {
				if (maze[v->y - 1][v->x].d_flag && !visited[v->y - 1][v->x]) { // if ( S.top has an unvisited adjacent node )
					if (flag) {
						push(&Visited_Path_by_DFS, v);    // Visited_Path에 push();
						tmp = peek(&Visited_Path_by_DFS);
						tmp->junction = true;;  // 분기점에서 edge를 끊어낸다.
					}
					push(&Shortest_Path_by_DFS, v);        // Shortest_Path에 push();
					v = &maze[v->y - 1][v->x];
					flag = false;
					continue;
				}
			}
			wrong_visited[v->y][v->x] = true; // 잘못 들른 path라면, 
			push(&Visited_Path_by_DFS, v);   // Visited_Path에는 push()하고,
			v = pop(&Shortest_Path_by_DFS);  // Shortest_Path에서는 pop()한다.
			flag = true;  // 분기점 생성.
//		}
//		else {
//			push(&Visited_Path_by_DFS, v);
//			v = pop(&Shortest_Path_by_DFS); // pop();
//			flag = true;
//		}
	}

	StackNode* ptr = peek(&Visited_Path_by_DFS);
	while (ptr != NULL) {
		printf("(%d, %d)%d ", ptr->vertex_ptr->x, ptr->vertex_ptr->y, ptr->junction);
		ptr = ptr->link;
	}
	printf("\n");

	StackNode* ptr1 = peek(&Shortest_Path_by_DFS);
	while (ptr1 != NULL) {
		printf("(%d, %d)%d ", ptr1->vertex_ptr->x, ptr1->vertex_ptr->y, ptr1->junction);
		ptr1 = ptr1->link;
	}


	// 이차원 배열 동적할당 해제
	for (int i = 1; i < HEIGHT; i++) {
		visited[i] = NULL;
	}
	free(visited[0]);
	free(visited);

	for (int i = 0; i < HEIGHT; i++) {
		wrong_visited[i] = NULL;
	}
	free(wrong_visited[0]);
	free(wrong_visited);

}



vertex* Destination;
LinkedStackList Wrong_Visited_Path_by_BFS;

void BFS() {
	//start of user code
	//Search By BFS

	vertex* V;
	vertex* W;
	LinkedStackList Leaves;
	int** visited;
	int leaf_flag;
	int i;

	initQ(&Queue_for_BFS);
	initS(&Wrong_Visited_Path_by_BFS);
	initS(&Leaves);

	visited = (int**)malloc(sizeof(int*) * HEIGHT);
	for (int i = 0; i < HEIGHT; i++) {
		visited[i] = (int*)malloc(sizeof(int) * WIDTH);
	}
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			visited[i][j] = false;
		}
	}

	V = &maze[0][0];
	V->before = NULL;
	V->wrong_visited = false;
	enqueue(&Queue_for_BFS, V);

	while (1) {
		V = dequeue(&Queue_for_BFS);
		visited[V->y][V->x] = true;
		leaf_flag = true;
		if ((V->y == HEIGHT - 1) && (V->x == WIDTH - 1)) { // 출구면 탐색 종료
			Destination = V;
			break;
		}
		//우, 하, 좌, 상 vertex 중 edge로 연결됬고 방문한적없는 모든 vertex를 queue에 add
		//W=right
		if (V->r_flag && !visited[V->y][V->x + 1]) {
			enqueue(&Queue_for_BFS, &maze[V->y][V->x + 1]);
			(&maze[V->y][V->x + 1])->before = V;
			leaf_flag = false;
		}
		//W=down;
		if (V->d_flag && !visited[V->y + 1][V->x]) {
			enqueue(&Queue_for_BFS, &maze[V->y + 1][V->x]);
			(&maze[V->y + 1][V->x])->before = V;
			leaf_flag = false;
		}
		//W=left
		if (V->x > 0 && maze[V->y][V->x - 1].r_flag && !visited[V->y][V->x - 1]) {
			enqueue(&Queue_for_BFS, &maze[V->y][V->x - 1]);
			(&maze[V->y][V->x - 1])->before = V;
			leaf_flag = false;
		}
		//W=up
		if (V->y > 0 && maze[V->y - 1][V->x].d_flag && !visited[V->y - 1][V->x]) {
			enqueue(&Queue_for_BFS, &maze[V->y - 1][V->x]);
			(&maze[V->y - 1][V->x])->before = V;
			leaf_flag = false;
		}
		if (leaf_flag == true) { // 더이상 진행이 불가능하다면, 그 vertex를 따로 저장(linked list 상에)
			push(&Leaves, V);
		}
	}
	while (V->before != NULL) { //탈출경로인 vertex들의 notpath값을 false로 변경.
		V->wrong_visited = false; //false이면 path로 정의, visual studio에선 구조체 내부 boolean은 기본으로 true로 설정되어있음.
		//printf("<%d, %d> ", V->x, V->y);
		V = V->before;
	}
	//printf("\n\n");

	if (!isS_empty(&Leaves)) {
		V = pop(&Leaves); //탐색경로들을 저장하는 과정. 더이상 진행이 불가능했던 vertex와 그 vertex 이전 vertex, 그리고 탈출경로인지의 여부를 이용한다.
	}
	while (1) {
		push(&Wrong_Visited_Path_by_BFS, V);
		if (V->before->wrong_visited == true)
			V = V->before;
		else {
			StackNode* tmp;
			push(&Wrong_Visited_Path_by_BFS, V->before);
			tmp = peek(&Wrong_Visited_Path_by_BFS);
			tmp->junction = true;
			if (isS_empty(&Leaves))
				break;
			V = pop(&Leaves);
		}
	}
	printf("!!!!!!!!\n");
	for (int i = 1; i < HEIGHT; i++) {
		for (int j = 1; j < WIDTH; j++) {
			printf("(%d, %d) ", maze[i][j].before->x, maze[i][j].before->y);
		}
		printf("\n");
	}

	StackNode* ptr = peek(&Wrong_Visited_Path_by_BFS);
	while (ptr != NULL) {
		printf("(%d, %d)%d ", ptr->vertex_ptr->x, ptr->vertex_ptr->y, ptr->junction);
		ptr = ptr->link;
	}
	printf("\n");

	//StackNode* ptr1 = peek(&Shortest_Path_by_BFS);
	//while (ptr1 != NULL) {
	//	printf("(%d, %d)%d ", ptr1->vertex_ptr->x, ptr1->vertex_ptr->y, ptr1->junction);
	//	ptr1 = ptr1->link;
	//}

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			printf("%d ", visited[i][j]);
		}
		printf("\n");
	}

	printf("(%d, %d) ", Destination->y, Destination->x);




	for (i = 1; i < HEIGHT; i++)
		visited[i] = NULL;
	free(visited[0]);
	free(visited);
	//end of user code

	return;
}

void main() {
	readFile();
	BFS();
}