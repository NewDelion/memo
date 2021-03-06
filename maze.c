#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define WIDTH 37
#define HEIGHT 21

//http://www.kumei.ne.jp/c_lang/intro/no_59.htm
int Locate(HANDLE hOut, int x, int y)
{
	COORD dwPos = { (SHORT)x, (SHORT)y };
	return SetConsoleCursorPosition(hOut, dwPos) == 0 ? -1 : 0;
}

// Command
// -1 => None
//  0 => Left
//  1 => Right
//  2 => Bottom
//  3 => Top
void InputKey(int *cmd){
	int key;
	if(kbhit() == 0){
		*cmd = -1;
		return;
	}
	key = getch();
	if(key == 'a' || key == 'A')
		*cmd = 0;
	else if(key == 'd' || key == 'D')
		*cmd = 1;
	else if(key == 's' || key == 'S')
		*cmd = 2;
	else if(key == 'w' || key == 'W')
		*cmd = 3;
	else
		*cmd = -1;
}

void InitializeMap(int map[WIDTH][HEIGHT]){
	int i,j;
	for(i=0;i<WIDTH;i++){
		for(j=0;j<HEIGHT;j++){
			if(i == 0 || j == 0 || i == WIDTH - 1 || j == HEIGHT - 1)
				map[i][j] = 1;
			else if(i % 2 == 0 && j % 2 == 0)
				map[i][j] = 1;
			//else if(i % 2 == 1 && j % 2 == 1)
			//	map[i][j] = 0;
			else
				map[i][j] = 0;
		}
	}
}

// 2 <= x && x <= WIDTH  - 3 && x % 2 == 0
// 2 <= y && y <= HEIGHT - 3 && y % 2 == 0
void KnockOver(int map[WIDTH][HEIGHT], int x, int y){
	int direction = rand() % 4;
	while(1){
		if(direction == 0 && map[x - 1][y] == 0){
			map[x - 1][y] = 1;
			break;
		}
		else if(direction == 1 && map[x + 1][y] == 0){
			map[x + 1][y] = 1;
			break;
		}
		else if(direction == 2 && map[x][y + 1] == 0){
			map[x][y + 1] = 1;
			break;
		}
		else if(direction == 3 && x == 2 && map[x][y - 1] == 0){
			map[x][y - 1] = 1;
			break;
		}
		direction = ++direction % 4;
	}
}

void CreateMap(int map[WIDTH][HEIGHT]){
	int i,j;
	InitializeMap(map);
	srand(time(NULL));
	for(i=2;i<WIDTH-2;i+=2){
		for(j=2;j<HEIGHT-2;j+=2){
			KnockOver(map, i, j);
		}
	}
}

void PrintMap(int map[WIDTH][HEIGHT], int sx, int sy, int gx, int gy, int px, int py){
	int i,j;
	system("cls");
	printf("経過時間：0.0秒\n");
	for(j=0;j<HEIGHT;j++){
		for(i=0;i<WIDTH;i++){
			if(i == px && j == py)
				printf("○");
			else if(i == sx && j == sy)
				printf("Ｓ");
			else if(i == gx && j == gy)
				printf("Ｇ");
			else if(map[i][j] == 0)
				printf("　");
			else if(map[i][j] == 1)
				printf("■");
		}
		printf("\n");
	}
	printf("\nW: ↑, S: ↓, A: ←, D: →");
}
void PrintResult(time_t start){
	int i;
	//クリアおめでとう！
	//Time：%.1f秒
	system("cls");
	for(i=0;i<20;i++)
		printf("■");
	printf("\n");
	printf("クリアおめでとう！\n");
	printf("Time: %.1f秒\n", difftime(time(NULL), start));
	for(i=0;i<20;i++)
		printf("■");
	printf("\n\npress any key...");
	getch();
}
void Move(HANDLE hOut, int map[WIDTH][HEIGHT], int sx, int sy, int px0, int py0, int px1, int py1){
	Locate(hOut, px0 * 2, py0 + 1);
	printf(sx == px0 && sy == py0 ? "Ｓ" : "　");
	Locate(hOut, px1 * 2, py1 + 1);
	printf("○");
}
void UpdateTotalTime(HANDLE hOut, time_t start){
	Locate(hOut, 10, 0);
	printf("%.1f秒", difftime(time(NULL), start));
}

int main(void){
	int i;
	int cmd, px = 1, py = 1;
	int map[WIDTH][HEIGHT];
	HANDLE hStdout;
	time_t start;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	start = time(NULL);
	CreateMap(map);
	PrintMap(map, 1, 1, WIDTH - 2, HEIGHT - 2, px, py);

	do{
		for(i=0;i<20;i++){
			InputKey(&cmd);
			switch(cmd){
				case 0:
					//printf("←");
					if(map[px - 1][py] == 0){
						Move(hStdout, map, 1, 1, px, py, px - 1, py);
						px--;
					}
					break;
				case 1:
					//printf("→");
					if(map[px + 1][py] == 0){
						Move(hStdout, map, 1, 1, px, py, px + 1, py);
						px++;
					}
					break;
				case 2:
					//printf("↓");
					if(map[px][py + 1] == 0){
						Move(hStdout, map, 1, 1, px, py, px, py + 1);
						py++;
					}
					break;
				case 3:
					//printf("↑");
					if(map[px][py - 1] == 0){
						Move(hStdout, map, 1, 1, px, py, px, py - 1);
						py--;
					}
					break;
			}
			if(px == WIDTH - 2 && py == HEIGHT - 2)
				break;
			Sleep(50);
		}
		UpdateTotalTime(hStdout, start);
	} while(px != WIDTH - 2 || py != HEIGHT - 2);
	PrintResult(start);
	return 0;
}