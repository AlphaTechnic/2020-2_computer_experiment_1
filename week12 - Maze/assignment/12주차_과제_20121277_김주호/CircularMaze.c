#include "CircularMaze.h"

void main() {
	int pre_wall = -1;
	int pre_set;
	/////////////////////
	int target_to_remove;
	int remove_num=0;

	fp = fopen("CircularMaze.maz", "wt");
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

	hori_walls = (int*)malloc(sizeof(int) * WIDTH);
	if (hori_walls == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}
	vert_walls = (int*)malloc(sizeof(int) * (WIDTH - 1));
	if (vert_walls == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}
	maze = (int*)calloc(WIDTH, sizeof(int)); // calloc : 0으로 초기화
	if (maze == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}

	// HEIGHT가 1이거나 WIDTH가 1인 (특별한) 경우의 처리.
	if (HEIGHT == 1) {
		// 맨 위쪽 수평wall 그리기
		for (int i = 0; i < WIDTH; i++) {
			fprintf(fp, "+-");
		}
		fprintf(fp, "+\n");

		// 수직 wall 그리기
		fprintf(fp, "|");
		for (int i = 0; i < WIDTH * 2 - 1; i++) {
			fprintf(fp, " ");
		}
		fprintf(fp, "|\n");

		// 바닥 wall 그리기
		for (int i = 0; i < WIDTH; i++) {
			fprintf(fp, "+-");
		}
		fprintf(fp, "+\n");
		printf("순환 미로 생성!\n");
		return;
	}
	else if (WIDTH == 1) {
		// 맨 위쪽 수평wall 그리기
		fprintf(fp, "+-+\n");

		// 수직 wall들과 바닥 wall 그리기
		for (int i = 0; i < HEIGHT - 1; i++) {
			fprintf(fp, "| |\n+ +\n");
		}
		fprintf(fp, "| |\n+-+\n");
		
		printf("순환 미로 생성!\n");
		return;
	}
	///////////////////////////////////////////////////////////////

	total_num_of_inside_walls = WIDTH * (HEIGHT - 1) + HEIGHT * (WIDTH - 1);

	wall_inside = (int*)malloc(total_num_of_inside_walls * sizeof(int)); // calloc : 0으로 초기화
	if (wall_inside == NULL) {
		printf("동적할당 err!\n");
		exit(1);
	}

	// 초기화
	for (int i = 0;i < total_num_of_inside_walls; i++) {
		wall_inside[i] = total_num_of_inside_walls;
	}
	//////////////////


	// 첫번째 line의 cell들을 방문하면서 set_number 부여하기
	srand((unsigned)time(NULL));
	for (int i = 0; i < WIDTH - 1; i++) {
		vert_walls[i] = rand() % 2; // 0 또는 1
		if (vert_walls[i]) {
			for (int j = (pre_wall + 1); j <= i; j++) {
				maze[j] = set_num;
			}
			/////
			wall_inside[n++] = wall_num;
			/////
			pre_wall = i;
			set_num++;
		}
		wall_num++;
	}

	for (int i = pre_wall + 1; i < WIDTH; i++) {
		maze[i] = set_num;
	}
	set_num++;
	// print_vert();


	for (int k = 1; k < HEIGHT - 1; k++) {
		make_vert_path();  // 수직 경로 만들고,
		//print_hori();      // 수평wall 그리고,
		make_hori_path();  // 수평 경로 만들고,
		//print_vert();      // 수직wall 그리고, 반복
	}

	// 마지막 line
	make_vert_path();
	//print_hori();

	// 마지막 line의 vert_wall 그리기.
	for (int i = 0; i < WIDTH - 1; i++) {
		if (maze[i] != maze[i + 1]) { // 다른 집합이 인접하였으면, 
			vert_walls[i] = 0;
			pre_set = maze[i + 1];
			maze[i + 1] = maze[i];
			for (int j = 0; j < WIDTH; j++) {
				if (maze[j] == pre_set) {
					maze[j] = maze[i];
				}
			}
		}
		else {
			vert_walls[i] = 1;
			////////////
			wall_inside[n++] = wall_num;
			////////////
		}
		wall_num++;
	}

	/* 마지막 line 수직wall과 바닥wall 그리기.
	print_vert();
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");
	*/

	// wall_inside 배열의 원소 중 일부를 지운다. (minimum(WIDTH, HEIGHT) / 2) 개를 그리지 않을 벽으로 바꿈.
	int count = 0;
	while(count < minimum(WIDTH, HEIGHT) / 2) {
		target_to_remove = rand() % n;
		if (wall_inside[target_to_remove] == -1) {
			continue;
		}
		else {
			wall_inside[target_to_remove] = -1;
			count++;
		}
	}

	// wall_inside 배열 상태 확인
	//for (int i = 0; i < total_num_of_inside_walls; i++) {
	//	printf("%d ", wall_inside[i]);
	//	if (i%WIDTH == WIDTH - 1) {
	//		printf("\n");
	//	}
	//}
	
	

	/// 출력 ///
	int k = 0;
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");

	for (int i = 0; i < 2*HEIGHT-1; i++) {
		if (i % 2 == 0) { // 수직
			fprintf(fp, "|");
			for (int j = 0; j < WIDTH - 1; j++) {
				if (wall_inside[k] == -1) { // 지워버린 벽은 그리지 않는다.
					for (; wall_inside[k] == -1; k++);
				}
				if ((2 * WIDTH - 1) * (i / 2) + j == wall_inside[k]) { // 수직wall을 그리는 순간
					fprintf(fp, " |");
					k++;
				}
				else {
					fprintf(fp, "  ");
				}
			}
			fprintf(fp, " |\n");
		}
		else { // 수평
			fprintf(fp, "+");
			for (int j = 0; j < WIDTH; j++) {
				if (wall_inside[k] == -1) { // 지워버린 벽은 그리지 않는다.
					for (; wall_inside[k] == -1; k++);
				}
				if (WIDTH - 1 + (2 * WIDTH - 1) * (i / 2) + j == wall_inside[k]) { // 수평wall을 그리는 순간
					fprintf(fp, "-+");
					k++;
				}
				else {
					fprintf(fp, " +");
				}
			}
			fprintf(fp, "\n");
		}
	}
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");
	fclose(fp);

	printf("순환 미로 생성!\n");

	return;
}

