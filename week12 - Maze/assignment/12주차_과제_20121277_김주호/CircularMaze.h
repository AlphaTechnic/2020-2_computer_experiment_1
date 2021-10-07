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
int* wall_inside; // 미로 내부에 세울(혹은 뉘일) wall의 위치를 저장.
int wall_num = 0;
int n = 0; // wall_inside의 index


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
	for (int i = 0; i < WIDTH; i++) {// cell들을 방문하면서, 바닥을 뚫을지 말지 결정
		hori_walls[i] = rand() % 2;
		if (!(hori_walls[i])) {
			drill_or_not = 1; // vertical 방향으로 뚫었다(drill)는 표식
		}

		if (i < WIDTH - 1) {
			if (pre != maze[i + 1]) {
				if (!drill_or_not) { // 적어도 하나는 바닥을 뚫어야 함.
					hori_walls[i] = 0;
				}
				else {
					drill_or_not = 0; // drill_or_not 0으로 set
				}
				pre = maze[i + 1];
			}
		}
		if ((i == WIDTH - 1) && !drill_or_not) { // line의 끝인데도 drill_or_not이 0으로 지정되면,
			hori_walls[i] = 0;
		}
		if (hori_walls[i]) { // 뚫지 않는 위치는 set_num을 다르게 설정
			maze[i] = set_num;
			set_num++;
			///////////////
			wall_inside[n++] = wall_num; // 뚫지 않는 위치에는 바닥_wall을 그리게 됨.
			///////////////
		}
		wall_num++;
	}
}

void make_hori_path() {
	int join_or_not = 0;
	int pre;

	for (int i = 0; i < WIDTH - 1; i++) {////////
		if (maze[i] != maze[i + 1]) { // 다른 집합이 인접한 경우에만, 결합(join)을 고려함.
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