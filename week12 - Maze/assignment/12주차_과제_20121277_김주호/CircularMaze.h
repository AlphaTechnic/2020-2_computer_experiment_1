#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE* fp;
int WIDTH, HEIGHT;
int total_num_of_inside_walls;
int* hori_walls, * vert_walls;
int* maze;
int set_num = 0;
//////////////////////
int* wall_inside; // �̷� ���ο� ����(Ȥ�� ����) wall�� ��ġ�� ����.
int wall_num = 0;
int n = 0; // wall_inside�� index


/*
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
*/

void make_vert_path() {
	int pre;
	int drill_or_not = 0;

	pre = maze[0];
	for (int i = 0; i < WIDTH; i++) {// cell���� �湮�ϸ鼭, �ٴ��� ������ ���� ����
		hori_walls[i] = rand() % 2;
		if (!(hori_walls[i])) {
			drill_or_not = 1; // vertical �������� �վ���(drill)�� ǥ��
		}

		if (i < WIDTH - 1) {
			if (pre != maze[i + 1]) {
				if (!drill_or_not) { // ��� �ϳ��� �ٴ��� �վ�� ��.
					hori_walls[i] = 0;
				}
				else {
					drill_or_not = 0; // drill_or_not 0���� set
				}
				pre = maze[i + 1];
			}
		}
		if ((i == WIDTH - 1) && !drill_or_not) { // line�� ���ε��� drill_or_not�� 0���� �����Ǹ�,
			hori_walls[i] = 0;
		}
		if (hori_walls[i]) { // ���� �ʴ� ��ġ�� set_num�� �ٸ��� ����
			maze[i] = set_num;
			set_num++;
			///////////////
			wall_inside[n++] = wall_num; // ���� �ʴ� ��ġ���� �ٴ�_wall�� �׸��� ��.
			///////////////
		}
		wall_num++;
	}
}

void make_hori_path() {
	int join_or_not = 0;
	int pre;

	for (int i = 0; i < WIDTH - 1; i++) {////////
		if (maze[i] != maze[i + 1]) { // �ٸ� ������ ������ ��쿡��, ����(join)�� �����.
			join_or_not = rand() % 2;
			if (join_or_not) {
				pre = maze[i + 1];
				maze[i + 1] = maze[i];
				for (int j = 0; j < WIDTH; j++)
					if (maze[j] == pre) {
						maze[j] = maze[i];
					}
				vert_walls[i] = 0;
			}
			else {
				vert_walls[i] = 1;
				wall_inside[n++] = wall_num;
			}
		}
		else {
			vert_walls[i] = 1;
			wall_inside[n++] = wall_num;
		}
		wall_num++;
	}
}

////////////////////////////////////////////////
int minimum(int x, int y) {
	if (x < y) {
		return x;
	}
	else {
		return y;
	}
}