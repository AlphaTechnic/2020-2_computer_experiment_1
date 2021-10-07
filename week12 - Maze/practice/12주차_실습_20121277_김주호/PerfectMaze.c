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
		printf("���� ���� err!\n");
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
		printf("�����Ҵ� err!\n");
		exit(1);
	}
	vert_wall = (int*)malloc(sizeof(int) * (WIDTH - 1));
	if (vert_wall == NULL) {
		printf("�����Ҵ� err!\n");
		exit(1);
	}
	maze = (int*)calloc(WIDTH, sizeof(int)); // calloc : 0���� �ʱ�ȭ
	if (maze == NULL) {
		printf("�����Ҵ� err!\n");
		exit(1);
	}

	// �� ���� ����wall �׸���
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");

	// ù��° line�� cell���� �湮�ϸ鼭 set_number �ο��ϱ�
	srand((unsigned)time(NULL));
	for (int i = 0; i < WIDTH - 1; i++) {
		vert_wall[i] = rand() % 2; // 0 �Ǵ� 1
		if (vert_wall[i]) {
			for (int j = (pre_wall + 1); j <= i; j++) {
				maze[j] = set_num;
			}
			pre_wall = i;
			set_num++;
		}
	}

	// HEIGHT�� 1�� (Ư����) ��쿡, line�� �糡 ����wall�� �ٴ�wall�� �׷��ְ� ����.
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

	// ����wall �׸���
	for (int i = pre_wall + 1; i < WIDTH; i++) {
		maze[i] = set_num;
	}
	set_num++;
	print_vert();


	for (int k = 1; k < HEIGHT - 1; k++) {
		make_vert_path();  // ���� ��� �����,
		print_hori();      // ����wall �׸���,
		make_hori_path();  // ���� ��� �����,
		print_vert();      // ����wall �׸���, �ݺ�
	}

	// ������ line
	make_vert_path();
	print_hori();

	// ������ line�� vert_wall �׸���.
	for (int i = 0; i < WIDTH - 1; i++) {
		if (maze[i] != maze[i + 1]) { // �ٸ� ������ �����Ͽ�����, 
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

	// ������ line ����wall�� �ٴ�wall �׸���.
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
			drill_or_not = 1; // vertical �������� �վ���(drill)�� ǥ��
		}

		
		if (i < WIDTH - 1) {
			if (pre != maze[i + 1]) {
				if (!drill_or_not) { // ��� �ϳ��� �ٴ��� �վ�� ��.
					hori_wall[i] = 0;
				}
				else {
					drill_or_not = 0; // drill_or_not 0���� set
				}
				pre = maze[i + 1];
			}
		}
		if ((i == WIDTH - 1) && !drill_or_not) { // line�� ���ε��� drill_or_not�� 0���� �����Ǹ�,
			hori_wall[i] = 0;
		}
		if (hori_wall[i]) { // ���� ���� ��ġ�� set_num�� �ٸ��� ����
			maze[i] = set_num;
			set_num++;
		}
	}
}

void make_hori_path() {
	int join_or_not = 0;
	int pre;

	for (int i = 0; i < WIDTH - 1; i++) {
		if (maze[i] != maze[i + 1]) { // �ٸ� ������ ������ ��쿡��, ����(join)�� �����.
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