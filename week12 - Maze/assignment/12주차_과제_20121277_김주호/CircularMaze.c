#include "CircularMaze.h"

void main() {
	int pre_wall = -1;
	int pre_set;
	/////////////////////
	int target_to_remove;
	int remove_num=0;

	fp = fopen("CircularMaze.maz", "wt");
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

	hori_walls = (int*)malloc(sizeof(int) * WIDTH);
	if (hori_walls == NULL) {
		printf("�����Ҵ� err!\n");
		exit(1);
	}
	vert_walls = (int*)malloc(sizeof(int) * (WIDTH - 1));
	if (vert_walls == NULL) {
		printf("�����Ҵ� err!\n");
		exit(1);
	}
	maze = (int*)calloc(WIDTH, sizeof(int)); // calloc : 0���� �ʱ�ȭ
	if (maze == NULL) {
		printf("�����Ҵ� err!\n");
		exit(1);
	}

	// HEIGHT�� 1�̰ų� WIDTH�� 1�� (Ư����) ����� ó��.
	if (HEIGHT == 1) {
		// �� ���� ����wall �׸���
		for (int i = 0; i < WIDTH; i++) {
			fprintf(fp, "+-");
		}
		fprintf(fp, "+\n");

		// ���� wall �׸���
		fprintf(fp, "|");
		for (int i = 0; i < WIDTH * 2 - 1; i++) {
			fprintf(fp, " ");
		}
		fprintf(fp, "|\n");

		// �ٴ� wall �׸���
		for (int i = 0; i < WIDTH; i++) {
			fprintf(fp, "+-");
		}
		fprintf(fp, "+\n");
		printf("��ȯ �̷� ����!\n");
		return;
	}
	else if (WIDTH == 1) {
		// �� ���� ����wall �׸���
		fprintf(fp, "+-+\n");

		// ���� wall��� �ٴ� wall �׸���
		for (int i = 0; i < HEIGHT - 1; i++) {
			fprintf(fp, "| |\n+ +\n");
		}
		fprintf(fp, "| |\n+-+\n");
		
		printf("��ȯ �̷� ����!\n");
		return;
	}
	///////////////////////////////////////////////////////////////

	total_num_of_inside_walls = WIDTH * (HEIGHT - 1) + HEIGHT * (WIDTH - 1);

	wall_inside = (int*)malloc(total_num_of_inside_walls * sizeof(int)); // calloc : 0���� �ʱ�ȭ
	if (wall_inside == NULL) {
		printf("�����Ҵ� err!\n");
		exit(1);
	}

	// �ʱ�ȭ
	for (int i = 0;i < total_num_of_inside_walls; i++) {
		wall_inside[i] = total_num_of_inside_walls;
	}
	//////////////////


	// ù��° line�� cell���� �湮�ϸ鼭 set_number �ο��ϱ�
	srand((unsigned)time(NULL));
	for (int i = 0; i < WIDTH - 1; i++) {
		vert_walls[i] = rand() % 2; // 0 �Ǵ� 1
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
		make_vert_path();  // ���� ��� �����,
		//print_hori();      // ����wall �׸���,
		make_hori_path();  // ���� ��� �����,
		//print_vert();      // ����wall �׸���, �ݺ�
	}

	// ������ line
	make_vert_path();
	//print_hori();

	// ������ line�� vert_wall �׸���.
	for (int i = 0; i < WIDTH - 1; i++) {
		if (maze[i] != maze[i + 1]) { // �ٸ� ������ �����Ͽ�����, 
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

	/* ������ line ����wall�� �ٴ�wall �׸���.
	print_vert();
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");
	*/

	// wall_inside �迭�� ���� �� �Ϻθ� �����. (minimum(WIDTH, HEIGHT) / 2) ���� �׸��� ���� ������ �ٲ�.
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

	// wall_inside �迭 ���� Ȯ��
	//for (int i = 0; i < total_num_of_inside_walls; i++) {
	//	printf("%d ", wall_inside[i]);
	//	if (i%WIDTH == WIDTH - 1) {
	//		printf("\n");
	//	}
	//}
	
	

	/// ��� ///
	int k = 0;
	for (int i = 0; i < WIDTH; i++) {
		fprintf(fp, "+-");
	}
	fprintf(fp, "+\n");

	for (int i = 0; i < 2*HEIGHT-1; i++) {
		if (i % 2 == 0) { // ����
			fprintf(fp, "|");
			for (int j = 0; j < WIDTH - 1; j++) {
				if (wall_inside[k] == -1) { // �������� ���� �׸��� �ʴ´�.
					for (; wall_inside[k] == -1; k++);
				}
				if ((2 * WIDTH - 1) * (i / 2) + j == wall_inside[k]) { // ����wall�� �׸��� ����
					fprintf(fp, " |");
					k++;
				}
				else {
					fprintf(fp, "  ");
				}
			}
			fprintf(fp, " |\n");
		}
		else { // ����
			fprintf(fp, "+");
			for (int j = 0; j < WIDTH; j++) {
				if (wall_inside[k] == -1) { // �������� ���� �׸��� �ʴ´�.
					for (; wall_inside[k] == -1; k++);
				}
				if (WIDTH - 1 + (2 * WIDTH - 1) * (i / 2) + j == wall_inside[k]) { // ����wall�� �׸��� ����
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

	printf("��ȯ �̷� ����!\n");

	return;
}

