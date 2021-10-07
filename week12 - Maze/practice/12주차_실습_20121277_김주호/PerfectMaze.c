#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_vert();
void print_hori();
void make_vert_path();
void make_hori_path();

FILE* fp;
int WIDTH, HEIGHT;
int* hori_wall, * vert_wall;
int* maze;
int set_num = 0;

void main() {
	int pre_wall = -1;
	int pre_set;

	fp = fopen("PerfectMaze.maz", "wt");
	if (fp == NULL) {
		printf("파일 쓰기 err!\n");
		return;
	}

	printf("Width : ");
	if (scanf("%d", &WIDTH) == -1) {
		return;
	}
	printf("Height : ");
	if (scanf("%d", &HEIGHT) == -1) {
		return;
	}

	hori_wall = (int*)malloc(sizeof(int) * WIDTH);
	if (hori_wall == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}
	vert_wall = (int*)malloc(sizeof(int) * (WIDTH - 1));
	if (vert_wall == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}
	maze = (int*)calloc(WIDTH, sizeof(int)); // calloc : 0으로 초기화
	if (maze == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}

	// 맨 위쪽 수평wall 그리기
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");

	// 첫번째 line의 cell들을 방문하면서 set_number 부여하기
	srand((unsigned)time(NULL));
	for (int i = 0; i < WIDTH - 1; i++) {
		vert_wall[i] = rand() % 2; // 0 또는 1
		if (vert_wall[i]) {
			for (int j = (pre_wall + 1); j <= i; j++) {
				maze[j] = set_num;
			}
			pre_wall = i;
			set_num++;
		}
	}

	// HEIGHT가 1인 (특별한) 경우에, line의 양끝 수직wall과 바닥wall을 그려주고 종료.
	if (HEIGHT == 1) {
		fprintf(fp, "|");
		for (int i = 0; i < WIDTH * 2 - 1; i++) {
			fprintf(fp, " ");
		}
		fprintf(fp, "|\n");
		for (int i = 0; i < WIDTH; i++) {
			fprintf(fp, "+-");
		}
		fprintf(fp, "+\n");
		return;
	}
	///////////////////////////////////////////////////////////////

	// 수직wall 그리기
	for (int i = pre_wall + 1; i < WIDTH; i++) {
		maze[i] = set_num;
	}
	set_num++;
	print_vert();


	for (int k = 1; k < HEIGHT - 1; k++) {
		make_vert_path();  // 수직 경로 만들고,
		print_hori();      // 수평wall 그리고,
		make_hori_path();  // 수평 경로 만들고,
		print_vert();      // 수직wall 그리고, 반복
	}

	// 마지막 line
	make_vert_path();
	print_hori();

	// 마지막 line의 vert_wall 그리기.
	for (int i = 0; i < WIDTH - 1; i++) {
		if (maze[i] != maze[i + 1]) { // 다른 집합이 인접하였으면, 
			vert_wall[i] = 0;
			pre_set = maze[i + 1];
			maze[i + 1] = maze[i];
			for (int j = 0; j < WIDTH; j++) {
				if (maze[j] == pre_set) {
					maze[j] = maze[i];
				}
			}
		}
		else {
			vert_wall[i] = 1;
		}
	}

	// 마지막 line 수직wall과 바닥wall 그리기.
	print_vert();
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");
}

void print_vert() {
	fprintf(fp, "|");
	for (int i = 0; i < WIDTH - 1; i++) {
		fprintf(fp, " ");
		if (vert_wall[i] == 1) {
			fprintf(fp, "|");
		}
		else fprintf(fp, " ");
	}
	fprintf(fp, " |\n");
}

void print_hori() {
	fprintf(fp, "+");
	for (int i = 0; i < WIDTH; i++) {
		if (hori_wall[i] == 1) {
			fprintf(fp, "-+");
		}
		else fprintf(fp, " +");
	}
	fprintf(fp, "\n");
}

void make_vert_path() {
	int pre;
	int drill_or_not = 0;

	pre = maze[0];
	for (int i = 0; i < WIDTH; i++) {
		hori_wall[i] = rand() % 2;
		if (!(hori_wall[i])) {
			drill_or_not = 1; // vertical 방향으로 뚫었다(drill)는 표식
		}

		
		if (i < WIDTH - 1) {
			if (pre != maze[i + 1]) {
				if (!drill_or_not) { // 적어도 하나는 바닥을 뚫어야 함.
					hori_wall[i] = 0;
				}
				else {
					drill_or_not = 0; // drill_or_not 0으로 set
				}
				pre = maze[i + 1];
			}
		}
		if ((i == WIDTH - 1) && !drill_or_not) { // line의 끝인데도 drill_or_not이 0으로 지정되면,
			hori_wall[i] = 0;
		}
		if (hori_wall[i]) { // 뚫지 않은 위치는 set_num을 다르게 설정
			maze[i] = set_num;
			set_num++;
		}
	}
}

void make_hori_path() {
	int join_or_not = 0;
	int pre;

	for (int i = 0; i < WIDTH - 1; i++) {
		if (maze[i] != maze[i + 1]) { // 다른 집합이 인접한 경우에만, 결합(join)을 고려함.
			join_or_not = rand() % 2;
			if (join_or_not) {
				pre = maze[i + 1];
				maze[i + 1] = maze[i];
				for (int j = 0; j < WIDTH; j++)
					if (maze[j] == pre) {
						maze[j] = maze[i];
					}
				vert_wall[i] = 0;
			}
			else {
				vert_wall[i] = 1;
			}
		}
		else {
			vert_wall[i] = 1;
		}
	}
}