#pragma once
#define LEN 30
#define COL 9
#define ROW 9
#define MINENUM 5
#define BOMB 1
#define SAFE 0
#define HEIGHT 90
typedef int Statu;
typedef struct coordinate {
	int x;
	int y;
}Coor;
typedef struct point {
	bool isOpen;   //格子是否被翻开
	int mineNums;//格子周围的雷数
	bool hasFlag;    //是否被插旗
}Point;
typedef struct grade {
	int time;
	char name[10];
}Grade;

void StartPage(void);
void Game(void);
void Score(void);
void ShowAllMine();
int SumMine(int x, int y);
void InitMine();
void PrintBox(void);
Statu Cheak(int i, int j);
void ToFlag();
void Lose();
int OpenNum(void);
void Win(int time);
void PutScore();
