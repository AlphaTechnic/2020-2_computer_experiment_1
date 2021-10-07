#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define true 1
#define false 0

// �̷� �׸��� ����(size ����). 1�� ����. ���̰� �ʹٸ� �� 0~1 / �ø��� �ʹٸ� - 1 �̻�
float magnification = 0.5;
// (translation, translation) ��ŭ �̷θ� �����̵��Ѵ�. ex ) 1 �Ҵ� -> (1,1) ��ŭ �̷θ� �����̵�.
float translation = 1;
// �̷� ���� ����
int thickness = 2;

float tune(float num) {
	return magnification * num + translation;
}

typedef struct vertex { // vertex�� ������ �ִ� �ڷᱸ��
	int x; int y;
	int r_flag; int d_flag;
	int wrong_visited;
	struct vertex* before;
}vertex;

typedef struct QueueNode { // QueueNode
	struct vertex* vertex_ptr;
	struct QueueNode* link; // DFS Ž���� Ž���ߴ� ��� ������ ���� ����ϴ� ����.
	int have_edge;
}QueueNode;

typedef struct LinkedQueueList { // ���Ḯ��Ʈ�� ������ ������ ����
	struct QueueNode* front, * rear;
}LinkedQueueList;

FILE* fp;
const char* filename;
int file_flag = false;
vertex** maze = NULL; // ������ �迭 ����

int HEIGHT, WIDTH;
int draw_flag = false;

int readFile();

void initQ(LinkedQueueList* q);
int isQ_empty(LinkedQueueList* q);
void enqueue(LinkedQueueList* q, vertex* vertex_ptr);
vertex* dequeue(LinkedQueueList* q);

void freeMemory();
///////////////////////////////2���� �ǽ�(DFS)///////////////////////////////////////

int dfs_flag = false;

typedef struct StackNode { // vertex_ptr�� data�� ������ �ִ� StackNode ����ü
	struct vertex* vertex_ptr;
	struct StackNode* link;
	int junction; // dfs Ž���� �ִܰ�ΰ� �ƴ϶�� ������ �Ǹ�, edge�� ���´�.
}StackNode;

typedef struct LinkedStackList { // ���Ḯ��Ʈ�� ������ ������ ����
	struct StackNode* front, * rear;
}LinkedStackList;

LinkedStackList Shortest_Path_by_DFS; // �ִ� ����� Ʈ���� ����
LinkedStackList Visited_Path_by_DFS; // Ʈ���� �湮 ��� �� �ִ� ��θ� ������ �κ�Ʈ�� ����

void initS(LinkedStackList* s);
void push(LinkedStackList* s, vertex* vertex_ptr);
vertex* pop(LinkedStackList* s);
int isS_empty(LinkedStackList* s);
StackNode* peek(LinkedStackList* s);
void DFS();
//////////////////////////////////2���� ����(BFS)/////////////////////////////////

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
	vertex* current_line; // 1���� �迭
	vertex* tmp_vertices_arr; // 1���� �迭


	if ((fp = fopen("PerfectMaze.maz", "r")) == NULL) {
		printf("file open err!");
		return false;
	}

	HEIGHT = 0;
	WIDTH = 0;

	if (fscanf(fp, "%c", &ch) == -1) { // '+' ����
		printf("scanf err!");
		return false;
	}
	while (1) { // �̷��� õ���� �����鼭 WIDTH�� �˾Ƴ�.		
		// '-' ����
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}

		if (ch == 10) { // ���๮�� ������ Ż�� (10 means "\n")
			break;
		}

		// '+' ����
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
	enqueue(&Queue_for_vertex_arr, current_line); // Queue�� ����

	while (1) { // 2���� �۾� :  1. cell�� �д´�. 2. �ٴ��� �д´�.
		floor = 1; // �̷��� �� �عٴ����� ǥ���ϴ� ����. �ո� �ٴ��� ������ 0�� �Ҵ�ǵ��� ����.
		if (fscanf(fp, "%c", &ch) == -1) { // '|' ����.
			printf("scanf err!");
			return false;
		}

		// 1. cell���� �д´�.
		for (int i = 0; i < WIDTH; i++) {
			current_line[i].x = i;
			current_line[i].y = HEIGHT;

			// ' ' �д´�.
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}

			// '|'(124)(������ �� ����) �Ǵ� ' '(32)(������ �� ����)�� ���´�.
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}
			if (ch == 32) { // ' ' ���, 
				current_line[i].r_flag = 1;
			}
			else if (ch == 124) { // '|' �̶��,
				current_line[i].r_flag = 0;
			}
		}
		current_line[WIDTH - 1].r_flag = 0;

		// ������ '\n'�� ����.
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}

		///////////////////////////////////////////////////////
		// 2. �ٴ��� �д´�. �� ó�� �����ϴ� '+'�� ����.
		if (fscanf(fp, "%c", &ch) == -1) {
			printf("scanf err!");
			return false;
		}

		// '-'(45)(�ٴ� ����) �Ǵ� ' '(32)(�ٴ� �ո�)�� ���´�. ('+'�� ������.)
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
			if (ch == 32) { // �Ʒ��� �շ��ִµ�,(32 means 'space')
				if (floor) { // '�ٴ�'�̶�� �Ǿ�������,
					floor = 0; // '�ٴ��� �ƴϴ�'��� floor ������ ����
				}
				current_line[i].d_flag = 1;
			}
			else if (ch == 45) {
				current_line[i].d_flag = 0;
			}
			// '+'�� ������.
			if (fscanf(fp, "%c", &ch) == -1) {
				printf("scanf err!");
				return false;
			}
		}

		if (floor) { // �̷��� �� �عٴڿ� �Դٸ�,
			for (int i = 0; i < WIDTH; i++) {
				current_line[i].d_flag = 0;
			}
			break;
		}

		current_line = tmp_vertices_arr;
		tmp_vertices_arr = NULL;

		// ������ ���� ���๮�� '\n'�� ������.
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

	// Queue�� ��� cell ������ maze 2���� �迭�� ����.
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
	// maze ������ �迭 ���.
	if (maze != NULL) {
		for (int i = 0; i < HEIGHT; i++) {
			free(maze[i]);
		}
		free(maze);
		maze = NULL;
	}

	// Queue�� ���.
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

//////////////////////// ���� ADT //////////////////////////
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

	int** wrong_visited; // �б��� ~ LeafNode������ ǥ�� (�߸� �鸥 path)
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
		if ((v->y == HEIGHT - 1) && (v->x == WIDTH - 1)) { // ���� ���� : ������ ����
			push(&Shortest_Path_by_DFS, v);
			break;
		}
//		if (!wrong_visited[v->y][v->x]) {
			// ������
			if ((v->r_flag) && !visited[v->y][v->x + 1]) { // if ( S.top has an unvisited adjacent node )
				if (flag) {
					push(&Visited_Path_by_DFS, v);     // Visited_Path�� push();
					tmp = peek(&Visited_Path_by_DFS);
					tmp->junction = true; // �б������� edge�� �����.
				}
				push(&Shortest_Path_by_DFS, v);    // Shortest_Path�� push();
				v = &maze[v->y][v->x + 1];
				flag = false;
				continue;
			}
			// �Ʒ�
			if ((v->d_flag) && !visited[v->y + 1][v->x]) { // if ( S.top has an unvisited adjacent node )
				if (flag) {
					push(&Visited_Path_by_DFS, v);  // Visited_Path�� push();
					tmp = peek(&Visited_Path_by_DFS);
					tmp->junction = true;  // �б������� edge�� �����.
				}
				push(&Shortest_Path_by_DFS, v);  // Shortest_Path�� push();
				v = &maze[v->y + 1][v->x];
				flag = false;
				continue;
			}
			// ����
			if (v->x > 0) {
				if (maze[v->y][v->x - 1].r_flag && !visited[v->y][v->x - 1]) { // if ( S.top has an unvisited adjacent node )
					if (flag) {
						push(&Visited_Path_by_DFS, v);    // Visited_Path�� push();
						tmp = peek(&Visited_Path_by_DFS);
						tmp->junction = true;  // �б������� edge�� �����.
					}
					push(&Shortest_Path_by_DFS, v);        // Shortest_Path�� push();
					v = &maze[v->y][v->x - 1];
					flag = false;
					continue;
				}
			}
			// ��
			if (v->y > 0) {
				if (maze[v->y - 1][v->x].d_flag && !visited[v->y - 1][v->x]) { // if ( S.top has an unvisited adjacent node )
					if (flag) {
						push(&Visited_Path_by_DFS, v);    // Visited_Path�� push();
						tmp = peek(&Visited_Path_by_DFS);
						tmp->junction = true;;  // �б������� edge�� �����.
					}
					push(&Shortest_Path_by_DFS, v);        // Shortest_Path�� push();
					v = &maze[v->y - 1][v->x];
					flag = false;
					continue;
				}
			}
			wrong_visited[v->y][v->x] = true; // �߸� �鸥 path���, 
			push(&Visited_Path_by_DFS, v);   // Visited_Path���� push()�ϰ�,
			v = pop(&Shortest_Path_by_DFS);  // Shortest_Path������ pop()�Ѵ�.
			flag = true;  // �б��� ����.
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


	// ������ �迭 �����Ҵ� ����
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
		if ((V->y == HEIGHT - 1) && (V->x == WIDTH - 1)) { // �ⱸ�� Ž�� ����
			Destination = V;
			break;
		}
		//��, ��, ��, �� vertex �� edge�� ������ �湮�������� ��� vertex�� queue�� add
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
		if (leaf_flag == true) { // ���̻� ������ �Ұ����ϴٸ�, �� vertex�� ���� ����(linked list ��)
			push(&Leaves, V);
		}
	}
	while (V->before != NULL) { //Ż������ vertex���� notpath���� false�� ����.
		V->wrong_visited = false; //false�̸� path�� ����, visual studio���� ����ü ���� boolean�� �⺻���� true�� �����Ǿ�����.
		//printf("<%d, %d> ", V->x, V->y);
		V = V->before;
	}
	//printf("\n\n");

	if (!isS_empty(&Leaves)) {
		V = pop(&Leaves); //Ž����ε��� �����ϴ� ����. ���̻� ������ �Ұ����ߴ� vertex�� �� vertex ���� vertex, �׸��� Ż���������� ���θ� �̿��Ѵ�.
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