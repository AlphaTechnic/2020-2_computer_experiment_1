#include "tetris.h"

static struct sigaction act, oact;

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	createRankList();
	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
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

	nextBlock[0] = rand() % 7;
	nextBlock[1] = rand() % 7;
	nextBlock[2] = rand() % 7;
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
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK");
	DrawBox(3, WIDTH + 10, 4, 8);
	move(8, WIDTH + 10);
	DrawBox(9, WIDTH + 10, 4, 8); // 두번째 nextblock을 보여주는 공간 그려줌.

	/* score를 보여주는 공간의 태두리를 그린다.*/
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

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int i, j, check = 1;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				// 쌓인 block이 있으면
				if (f[blockY + i][blockX + j]) {
					check = 0;
					return check;
				}
				// 필드 밖이면
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


	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
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

	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][pre_rot][i][j] == 1) {
				if (i + pre_y >= 0) {
					move(pre_y + i + 1, pre_x + j + 1);
					printw(".");
				}
				if (i + pre_shadow >= 0) { // 이전 그림자를 지움.
					move(i + pre_shadow + 1, j + pre_x + 1);
					printw(".");
				}
			}
		}
	}

	//3. 새로운 블록 정보를 그린다. 
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

		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7;

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
				if (blockY + i + 1 == HEIGHT || f[blockY + i + 1][blockX + j] == 1) { // 바닥과 닿거나 다른 블럭과 닿았을 때
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
	// 1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	for (i = 1; i < HEIGHT; i++) {
		check = 1;
		for (j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) {
				check = 0;
				break;
			}
		}
		// 2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
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
	while (CheckToMove(field, blockID, blockRotate, y+1, x)) {
		y++;
	}
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	
	// 블럭과 그림자가 포개질 때, '/'가 아닌 ' '로(그림자가 아닌 블럭으로) field에 그려주게 된다.
	DrawBlock(y, x, blockID, blockRotate, ' '); 
	}

void createRankList() {
	FILE* fp = fopen("rank.txt", "r");
	if (fp == NULL) {
		head = NULL;
		return;
	}

	fscanf(fp, "%d", &size_of_ranks);
	Node* pre;

	for (int i = 0; i < size_of_ranks; i++) {
		if (head == NULL) {
			head = (Node*)malloc(sizeof(Node));
			fscanf(fp, "%s %d", head->name, &(head->score));
			head->link = NULL;
		}
		
		else {
			cur_node = (Node*)malloc(sizeof(Node));
			fscanf(fp, "%s %d", cur_node->name, &(cur_node->score));
			if (cur_node->score > head->score) {
				cur_node->link = head;
				head = cur_node;
			}
			else {
				pre = head;
				while (pre->link != NULL) {
					if ((cur_node->score) >= (pre->score)) {
						cur_node->link = pre->link;
						pre->link = cur_node;
						break;
					}
					pre = pre->link;
				}
				if (pre->link == NULL) {
					pre->link = cur_node;
					cur_node->link = NULL;
				}
			}
		}
	}
	fclose(fp);
	
}


void rank() {
	// user code
	int count;
	int end;
	int X = -858993460, Y = -858993460;
	char search_name[NAMELEN];
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	Node* check = head;

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

		if (X == -858993460 && Y == -858993460) { // 둘다 입력하지 않았다면,
			count = 0;
			Y = size_of_ranks;
			X = 1;

			for (int i = 0; i < size_of_ranks; i++) {
				// printw("%s!!!!!!!!!!!!!!!!!!!!!!", check->name);
				printw(" %-16s | %d \n", check->name, check->score);
				check = check->link;
				if (check == NULL) {
					break;
				}
			}
			getch();
		}

		else if (X == -858993460) { // Y만 입력했다면,
			end = (Y < size_of_ranks) ? Y : size_of_ranks;
			count = 0;
			X = 1;

			do {
				count++;
				if (count >= X) {
					if (count > end) break;
					printw(" %-16s | %d \n", check->name, check->score);
				}
				check = check->link;
			} while (check != NULL);
			getch();
		}

		else if (Y == -858993460) { // X만 입력했다면,
			count = 0;

			if (X <= 0 || X > size_of_ranks) {
				printw("\n\nsearch failure : no rank in the list\n");
				getch();
			}
			else {
				do {
					count++;
					if (count >= X) {
						if (count > size_of_ranks) break;
						printw(" %-16s | %d \n", check->name, check->score);
					}
					check = check->link;
				} while (check != NULL);
				getch();
			}
			getch();
		}

		else if (X > 0 && Y > 0 && X <= Y) { // 정상 입력,
			int end = (Y < size_of_ranks) ? Y : size_of_ranks;

			for (int i = X; i <= end; i++) {
				printw(" %-16s | %d \n", check->name, check->score);
				check = check->link;
				if (check == NULL) {
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
		break;

	case '3':
		echo();
		printw("input the rank you want to delete : ");
		break;
	}
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

	if (head == NULL) { // 리스트가 비어있으면,
		head = new_node;
	}
	else if (new_node->score > head->score) { // 신기록이라면,
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
		if (pre->link == NULL) { // 리스트의 끝에 도달하면,
			pre->link = new_node;
			new_node->link = NULL;
		}
	}
	size_of_ranks++;
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	// user code
}

int recommend(RecNode* root) {
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay() {
	// user code
}
