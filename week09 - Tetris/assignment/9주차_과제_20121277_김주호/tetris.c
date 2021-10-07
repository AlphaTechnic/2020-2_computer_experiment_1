#include "tetris.h"

static struct sigaction act, oact;

void DrawRecommended(int y, int x, int blockID, int blockRotate);

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	// �ʱ�ȭ
	recRoot = (RecNode*)malloc(sizeof(RecNode));
	recRoot->lv = -1;
	recRoot->score = 0;
	recRoot->f = field;
	gen_rec_tree(recRoot);

	createRankList();
	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_R_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit = 1; break;
		default: break;
		}
	}
	//writeRankFile();

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	int i, j;

	for (j = 0; j < HEIGHT; j++)
		for (i = 0; i < WIDTH; i++)
			field[j][i] = 0;

	for (i = 0; i < VISIBLE_BLOCKS; ++i) {
		nextBlock[i] = rand() % 7;
	}
	
	recommend();
	// recommend(recRoot);
	// nextBlock[0] = rand() % 7;
	// nextBlock[1] = rand() % 7;
	// nextBlock[2] = rand() % 7;

	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	score = 0;
	gameOver = 0;
	timed_out = 0;

	DrawOutline();
	DrawField();
	//DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {
	int i, j;
	/* ���� �������� ������ �µθ��� �׸���.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* next block�� �����ִ� ������ �µθ��� �׸���.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK");
	DrawBox(3, WIDTH + 10, 4, 8);
	move(8, WIDTH + 10);
	DrawBox(9, WIDTH + 10, 4, 8); // �ι�° nextblock�� �����ִ� ���� �׷���.

	/* score�� �����ִ� ������ �µθ��� �׸���.*/
	move(15, WIDTH + 10);
	printw("SCORE");
	DrawBox(16, WIDTH + 10, 1, 8);
}

int GetCommand() {
	int command;
	command = wgetch(stdscr);
	switch (command) {
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret = 1;
	int drawFlag = 0;
	switch (command) {
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
			blockRotate = (blockRotate + 1) % 4;
		break;
	case KEY_DOWN:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
			blockX++;
		break;
	case KEY_LEFT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
			blockX--;
		break;
	default:
		break;
	}
	if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
	return ret;
}

void DrawField() {
	int i, j;
	for (j = 0; j < HEIGHT; j++) {
		move(j + 1, 1);
		for (i = 0; i < WIDTH; i++) {
			if (field[j][i] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score) {
	move(17, WIDTH + 11);
	printw("%8d", score);
}

void DrawNextBlock(int* nextBlock) {
	int i, j;
	for (i = 0; i < 4; i++) {
		move(4 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[1]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for (i = 0; i < 4; i++) {
		move(10 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
	int i, j;
	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j = 0; j < height; j++) {
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}
	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////ù���� �ǽ����� �����ؾ� �� �Լ�/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int i, j, check = 1;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				// ���� block�� ������
				if (f[blockY + i][blockX + j]) {
					check = 0;
					return check;
				}
				// �ʵ� ���̸�
				if (blockY + i >= HEIGHT) {
					check = 0;
					return check;
				}
				if (blockX + j < 0) {
					check = 0;
					return check;
				}
				if (blockX + j >= WIDTH) {
					check = 0;
					return check;
				}
			}
		}
	}
	return check;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
	int pre_x = blockX, pre_y = blockY;
	int pre_rot = blockRotate;
	int i, j;

	int pre_shadow;


	//1. ���� ��� ������ ã�´�. ProcessCommand�� switch���� ������ ��
	switch (command) {
	case KEY_UP:
		pre_rot = (blockRotate + 3) % 4;
		break;
	case KEY_DOWN:
		pre_y--;
		break;
	case KEY_LEFT:
		pre_x++;
		break;
	case KEY_RIGHT:
		pre_x--;
		break;
	}
	pre_shadow = pre_y;
	while (CheckToMove(f, currentBlock, pre_rot, pre_shadow + 1, pre_x)) {
		pre_shadow++;
	}

	//2. ���� ��� ������ �����. DrawBlock�Լ� ������ ��.
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][pre_rot][i][j] == 1) {
				if (i + pre_y >= 0) {
					move(pre_y + i + 1, pre_x + j + 1);
					printw(".");
				}
				if (i + pre_shadow >= 0) { // ���� �׸��ڸ� ����.
					move(i + pre_shadow + 1, j + pre_x + 1);
					printw(".");
				}
			}
		}
	}

	//3. ���ο� ��� ������ �׸���. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	move(HEIGHT, WIDTH + 10);
}

void BlockDown(int sig) {
	int i, j;
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX) == 1) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else {
		if (blockY == -1) gameOver = 1;

		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		blockY = -1; blockX = (WIDTH / 2) - 2; blockRotate = 0;

		//nextBlock[0] = nextBlock[1];
		//nextBlock[1] = nextBlock[2];
		//nextBlock[2] = rand() % 7;
		for (i = 0; i < VISIBLE_BLOCKS - 1; ++i) {
			nextBlock[i] = nextBlock[i + 1];
		}
		nextBlock[VISIBLE_BLOCKS - 1] = rand() % 7;
		// recommend(recRoot);
		recommend();

		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	}
	timed_out = 0;

}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	/*
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
			}
		}
	}
	*/
	int i, j;
	int touched = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
				if (blockY + i + 1 == HEIGHT || f[blockY + i + 1][blockX + j] == 1) { // �ٴڰ� ��ų� �ٸ� ���� ����� ��
					touched++;
				}
			}
		}
	}
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	int i, j, k;
	int check;
	int count = 0;
	// 1. �ʵ带 Ž���Ͽ�, �� �� ������ �ִ��� Ž���Ѵ�.
	for (i = 1; i < HEIGHT; i++) {
		check = 1;
		for (j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) {
				check = 0;
				break;
			}
		}
		// 2. �� �� ������ ������ �ش� ������ �����. ��, �ش� �������� �ʵ尪�� ��ĭ�� ������.
		if (check == 1) {
			for (k = i; k > 0; k--) {
				for (int j = 0; j < WIDTH; j++) {
					f[k][j] = f[k - 1][j];
				}
			}
			count++;
		}
	}
	return count * count * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	while (CheckToMove(field, blockID, blockRotate, y + 1, x)) {
		y++;
	}
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawRecommended(recommendY, recommendX, nextBlock[0], 'R');
	DrawShadow(y, x, blockID, blockRotate);

	// ���� �׸��ڰ� ������ ��, '/'�� �ƴ� ' '��(�׸��ڰ� �ƴ� ������) field�� �׷��ְ� �ȴ�.
	DrawBlock(y, x, blockID, blockRotate, ' ');
}

void createRankList() {
	FILE* fp = fopen("rank.txt", "r");
	if (fp == NULL) {
		head = NULL;
		return;
	}

	fscanf(fp, "%d", &size_of_ranks);

	Node* ptr;
	for (int i = 0; i < size_of_ranks; i++) {
		if (head == NULL) {
			head = (Node*)malloc(sizeof(Node));
			fscanf(fp, "%s %d", head->name, &(head->score));
			head->link = NULL;
		}

		else {
			cur_node = (Node*)malloc(sizeof(Node));
			fscanf(fp, "%s %d", cur_node->name, &(cur_node->score));

			for (ptr = head; ptr->link != NULL; ptr = ptr->link);
			ptr->link = cur_node;
		}
	}
	fclose(fp);
}


void rank() {
	// for case1
	int count;
	int end;
	int X = -858993460, Y = -858993460;
	char search_name[NAMELEN];
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	Node* ptr = head;

	// for case2
	int flag = 0;

	// for case3
	int i;
	int rank_to_del;
	Node* tmp;

	switch (wgetch(stdscr)) {
	case '1':
		echo();
		printw("X : ");
		scanw("%d", &X);
		printw("Y : ");
		scanw("%d", &Y);
		noecho();
		printw("       name       |   score   \n");
		printw("------------------------------\n");

		if (X == -858993460 && Y == -858993460) { // �Ѵ� �Է����� �ʾҴٸ�,
			Y = size_of_ranks;
			X = 1;

			for (int i = 0; i < size_of_ranks; i++) {
				// printw("%s!!!!!!!!!!!!!!!!!!!!!!", check->name);
				printw(" %-16s | %d \n", ptr->name, ptr->score);
				ptr = ptr->link;
				if (tmp == NULL) {
					break;
				}
			}
			getch();
		}

		else if (X == -858993460) { // Y�� �Է��ߴٸ�,
			int end = (Y < size_of_ranks) ? Y : size_of_ranks;

			int i;
			for (int i = 0; i < end; i++) {
				printw(" %-16s | %d \n", ptr->name, ptr->score);
				ptr = ptr->link;
				if (ptr == NULL) {
					break;
				}
			}
			getch();
		}

		else if (Y == -858993460) { // X�� �Է��ߴٸ�,
			if (X <= 0 || X > size_of_ranks) {
				printw("\n\nsearch failure : no rank in the list\n");
				getch();
			}
			else {
				int end = size_of_ranks;

				int i;
				for (i = 0, ptr = head; i < X - 1; i++, ptr = ptr->link);
				for (int i = X; i <= end; i++) {
					printw(" %-16s | %d \n", ptr->name, ptr->score);
					ptr = ptr->link;
					if (ptr == NULL) {
						break;
					}
				}
				getch();
			}
			getch();
		}

		else if (X > 0 && Y > 0 && X <= Y) { // ���� �Է�,
			int end = (Y < size_of_ranks) ? Y : size_of_ranks;

			int i;
			for (i = 0, tmp = head; i < X - 1; i++, ptr = ptr->link);
			for (int i = X; i <= end; i++) {
				printw(" %-16s | %d \n", ptr->name, ptr->score);
				ptr = ptr->link;
				if (ptr == NULL) {
					break;
				}
			}
			getch();
		}
		else {
			printw("\n\nsearch failure : no rank in the list\n");
			getch();
		}
		break;
	case '2':
		echo();
		printw("input the name: ");
		scanw("%s", search_name);
		noecho();

		printw("       name       |   score   \n");
		printw("------------------------------\n");

		for (ptr = head; ptr; ptr = ptr->link) {
			if (strcmp(ptr->name, search_name) == 0) {
				printw(" %-16s | %d \n", ptr->name, ptr->score);
				flag = 1;
			}
		}
		if (flag == 0) {
			printw("search failure: no name in the list\n");
		}

		break;

	case '3':
		echo();
		printw("input the rank : ");
		scanw("%d", &rank_to_del);
		noecho();

		if (rank_to_del<1 || rank_to_del >size_of_ranks) { // ����� �Է� �����˻�
			printw("search failure: no rank in the list\n");
			break;
		}

		for (i = 1, ptr = head; i < rank_to_del - 1; i++, ptr = ptr->link); // ptr�� Ÿ������ pre���� �̵�
		if (i == 1 && rank_to_del != 2) { // head�� ���� ����̶��,
			head = ptr->link;
			free(ptr);
			size_of_ranks--;
			printw("result : the rank deleted\n");
			break;
		}
		else if (ptr->link->link == NULL) { // ���� �� ��尡 ���� ����̶��,
			tmp = ptr->link;
			ptr->link = NULL;
			free(tmp);
			size_of_ranks--;
		}
		else {
			tmp = ptr->link;
			ptr->link = ptr->link->link;
			free(tmp);
			size_of_ranks--;
		}
		printw("result : the rank deleted\n");
		writeRankFile(); // text ���ϵ� ������Ʈ
		break;

	default: break;
	}
	getch();
}


void writeRankFile() {
	FILE* fp2;
	fp2 = fopen("rank.txt", "w");
	fprintf(fp2, "%d\n", size_of_ranks);

	Node* cur_node = (Node*)malloc(sizeof(Node));
	cur_node = head;

	while (cur_node != NULL) {
		fprintf(fp2, "%s %d\n", cur_node->name, cur_node->score);
		cur_node = cur_node->link;
	}
	fclose(fp2);
}

void newRank(int score) {
	clear();
	Node* new_node = (Node*)malloc(sizeof(Node));
	Node* pre;
	char name[NAMELEN];

	new_node->score = score;
	new_node->link = NULL;

	printw("your name : ");
	echo();
	getstr(name);
	noecho();
	strcpy(new_node->name, name);

	if (head == NULL) { // ����Ʈ�� ���������,
		head = new_node;
	}
	else if (new_node->score > head->score) { // �ű���̶��,
		new_node->link = head;
		head = new_node;
	}
	else {
		pre = head;
		while (pre->link != NULL) {
			if (new_node->score >= pre->link->score) {
				new_node->link = pre->link;
				pre->link = new_node;
				break;
			}
			pre = pre->link;
		}
		if (pre->link == NULL) { // ����Ʈ�� ���� �����ϸ�,
			pre->link = new_node;
			new_node->link = NULL;
		}
	}
	size_of_ranks++;
	writeRankFile();
}

void gen_rec_tree(RecNode* root) {
	RecNode** c = root->c;

	for (int i = 0; i < CHILDREN_MAX; i++) {
		c[i] = (RecNode*)malloc(sizeof(RecNode));
		c[i]->lv = root->lv + 1;
		c[i]->f = (char(*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);

		if (c[i]->lv < VISIBLE_BLOCKS) {
			gen_rec_tree(c[i]);  // ��͸� �̿��� child ����Ʈ�� ����.
		}
	}
}

void DrawRecommended(int y, int x, int blockID, int blockRotate) {
	if (CheckToMove(field, nextBlock[0], recommendR, recommendY, recommendX)) {
		DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
	}
}

// �ǽ� recommend
int recommend_using_dfs(RecNode* root) {
	int max = 0; // �̸� ���̴� ���� ��õ ��ġ���� ������� �� ���� �� �ִ� �ִ� ����

	// user code
	int rot, x, y;
	int left, right;

	int h, w;
	int tmp;
	int rot_making_max, x_making_max, y_making_max;
	int flag = 0;
	int ind = 0;
	int lv;

	RecNode** c = root->c;
	lv = (root->lv) + 1;

	for (rot = 0; rot < NUM_OF_ROTATE; rot++) { // ��� ������ rotation�� ���Ͽ�
		// ��� ������ '��ϸ��'�� 'ȸ�� ��'�� ���Ͽ� 
		// blockX�� ���� �� �ִ� ���� ����ǥ(left)�� ������ ����ǥ(right)�� ���Ѵ�.
		switch (nextBlock[lv]) {
		case 0:
			switch (rot) {
			case 0:
				left = 0; right = 6;
				break;
			case 1:
				left = -1; right = 8;
				break;
			case 2:
				left = 0; right = 6;
				break;
			case 3:
				left = -1; right = 8;
				break;
			}
		case 1:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -2; right = 6;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 2:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -2; right = 6;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 3:
			switch (rot) {
			case 0:
				left = 0; right = 7;
				break;
			case 1:
				left = 0; right = 8;
				break;
			case 2:
				left = 0; right = 7;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 4:
			switch (rot) {
			case 0:
				left = -1; right = 7;
				break;
			case 1:
				left = -1; right = 7;
				break;
			case 2:
				left = -1; right = 7;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 5:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -1; right = 7;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 6:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -1; right = 7;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		}

		for (x = left; x <= right; x++) { // ����� ������ �� �ִ� ��� x�� ���Ͽ�
			// �ڽĿ��� ���� �θ��� �ʵ� ������ �ش�.
			for (h = 0; h < HEIGHT; h++) {
				for (w = 0; w < WIDTH; w++) {
					c[ind]->f[h][w] = root->f[h][w];
				}
			}

			// ����� �ٴڿ� ����� ���� y��ǥ�� ���Ѵ�.
			y = 0;
			if (CheckToMove(c[ind]->f, nextBlock[lv], rot, y, x)) { // ������ �� �ִٸ�,
				while (CheckToMove(c[ind]->f, nextBlock[lv], rot, ++y, x)); // ������ �� ���� ������ ������.
				--y;
			}
			else {
				continue;
			}

			// ���� (y, x)�� ���� ������ ���
			c[ind]->score = root->score + AddBlockToField(c[ind]->f, nextBlock[lv], rot, y, x) + DeleteLine(c[ind]->f);

			if (lv < VISIBLE_BLOCKS - 1) {
				tmp = recommend_using_dfs(c[ind]); // ��ͷ� recommend�� �ҷ��� �ڽ��� �ڽĳ�带 dfs Ž��
			}
			else {
				tmp = c[ind]->score;
			}

			if (max < tmp) {
				max = tmp; // max ����
				rot_making_max = rot;
				x_making_max = y;
				y_making_max = x;
				flag = 1; // max�� ���ŵǾ����� ���
			}
			ind++;
		}
	}

	if (flag && lv == 0) { // max ���ŵǰ�, lv == 0
		recommendR = rot_making_max;
		recommendY = x_making_max;
		recommendX = y_making_max;
	}
	////////////////////////////////////
	return max;
}

// prunning�� �̿��� ���� recommend
int recommend_by_pruning(char old_field[HEIGHT][WIDTH], int lv, int score) {
	int max = 0; // �̸� ���̴� ���� ��õ ��ġ���� ������� �� ���� �� �ִ� �ִ� ����

	// user code
	int rot, x, y;
	int left, right;

	int h, w;
	int tmp;
	int rot_making_max, x_making_max, y_making_max;
	int flag = 0;

	char new_field[HEIGHT][WIDTH];

	// ���°��� Ʈ���� �ܸ� ��忡 ����
	if (lv > VISIBLE_BLOCKS - 1) return score;

	for (rot = 0; rot < NUM_OF_ROTATE; rot++) { // ��� ������ rotation�� ���Ͽ�
		// ��� ������ '��ϸ��'�� 'ȸ�� ��'�� ���Ͽ� 
		// blockX�� ���� �� �ִ� ���� ����ǥ(left)�� ������ ����ǥ(right)�� ���Ѵ�.
		switch (nextBlock[lv]) {
		case 0:
			switch (rot) {
			case 0:
				left = 0; right = 6;
				break;
			case 1:
				left = -1; right = 8;
				break;
			case 2:
				left = 0; right = 6;
				break;
			case 3:
				left = -1; right = 8;
				break;
			}
		case 1:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -2; right = 6;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 2:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -2; right = 6;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 3:
			switch (rot) {
			case 0:
				left = 0; right = 7;
				break;
			case 1:
				left = 0; right = 8;
				break;
			case 2:
				left = 0; right = 7;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 4:
			switch (rot) {
			case 0:
				left = -1; right = 7;
				break;
			case 1:
				left = -1; right = 7;
				break;
			case 2:
				left = -1; right = 7;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 5:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -1; right = 7;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		case 6:
			switch (rot) {
			case 0:
				left = -1; right = 6;
				break;
			case 1:
				left = -1; right = 7;
				break;
			case 2:
				left = -1; right = 6;
				break;
			case 3:
				left = -1; right = 7;
				break;
			}
		}

		for (x = left; x <= right; x++) { // ����� ������ �� �ִ� ��� x�� ���Ͽ�
			// �ڽĿ��� ���� �θ��� �ʵ� ������ �ش�.
			for (h = 0; h < HEIGHT; h++) {
				for (w = 0; w < WIDTH; w++) {
					new_field[h][w] = old_field[h][w];
				}
			}

			// ����� �ٴڿ� ����� ���� y��ǥ�� ���Ѵ�.
			y = 0;
			if (CheckToMove(new_field, nextBlock[lv], rot, y, x)) { // ������ �� �ִٸ�,
				while (CheckToMove(new_field, nextBlock[lv], rot, ++y, x)); // ������ �� ���� ������ ������.
				--y;
			}
			else {
				continue;
			}

			// ���� (y, x)�� ���� ������ ���
			// field�� level, score�� �����Ͽ� ��ͷ� ȣ���Ѵ�.
			int new_score;
			if (lv < VISIBLE_BLOCKS/2 + 1) {
				tmp = recommend_by_pruning(new_field, lv + 1, score + AddBlockToField(new_field, nextBlock[lv], rot, y, x) + DeleteLine(new_field));

				if (max < tmp) {
					max = tmp; // max ����
					rot_making_max = rot;
					x_making_max = y;
					y_making_max = x;
					flag = 1; // max�� ���ŵǾ����� ���
				}
			}
			else if (lv >= VISIBLE_BLOCKS / 2 + 1) {
				new_score = score + AddBlockToField(new_field, nextBlock[lv], rot, y, x) + DeleteLine(new_field);
				if (new_score >= (VISIBLE_BLOCKS/2)* (VISIBLE_BLOCKS / 2) * 15) {
					tmp = recommend_by_pruning(new_field, lv + 1, new_score);

					if (max < tmp) {
						max = tmp; // max ����
						rot_making_max = rot;
						x_making_max = y;
						y_making_max = x;
						flag = 1; // max�� ���ŵǾ����� ���
					}
				}
				else {
					return score;
				}
			}
		}
	}
	if (flag && lv == 0) { // max ���ŵǰ�, lv == 0
		recommendR = rot_making_max;
		recommendY = x_making_max;
		recommendX = y_making_max;
	}
	////////////////////////////////////
	return max;
}

void stack_recommended_block(int sig) {
	int i;
	if (!CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX)) gameOver = 1;
	else {
		score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX);
		score += DeleteLine(field);
		blockY = -1; blockX = (WIDTH / 2) - 2; blockRotate = 0; //�ʱ�ȭ

		for (i = 0; i < VISIBLE_BLOCKS - 1; ++i) {
			nextBlock[i] = nextBlock[i + 1];
		}
		nextBlock[VISIBLE_BLOCKS - 1] = rand() % 7;
		
		// recommend(recRoot);
		recommend();
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
		DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
		timed_out = 0;
	}
}

void recommendedPlay() {
	// play() �Լ��� �����Ѵ�.
	int command;
	clear();
	// play() �Լ����� ������ �κ� 1. 
	// signal action�� BlockDown�� �ƴ� stack_recommended_block����
	act.sa_handler = stack_recommended_block;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (command == 'q' || command == 'Q') {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	// play() �Լ����� ������ �κ� 2. 
	// newRank(score); ����. ������ ��ŷ�� ��������� �ʴ´�.
}

int recommend() {
	int method;

	// ���1: ��� ���ɼ��� ��ȸ
	// method = recommend_using_dfs(recRoot);
	// ���2: prunning�� �̿��� ����ȭ
	 method = recommend_by_pruning(field,0,0);

	return method;
}
