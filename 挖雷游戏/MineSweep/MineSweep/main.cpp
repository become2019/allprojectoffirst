#include "kimika.h"
#include <stdio.h>
#include <graphics.h>
#include "resource.h"
#include <time.h>
Point mines[COL][ROW];
MOUSEMSG m;
int firstCheak;//�Ƿ��״�cheak
//-------------------------------------------
//ԭadd.cpp-------------
void ShowAllMine()
{
	for (int i = 0; i < COL; i++) {
		for (int j = 0; j < ROW; j++) {
			if (mines[i][j].mineNums == -1)
				mines[i][j].isOpen = true;
		}
	}
}

int SumMine(int x, int y)
{
	int count = 0;
	if (x - 1 >= 0 && y - 1 >= 0)//û�г�����Ե
		if (mines[x - 1][y - 1].mineNums == -1)
			count++;
	if (x - 1 >= 0)
		if (mines[x - 1][y].mineNums == -1)
			count++;
	if (x - 1 >= 0 && y + 1 < COL)
		if (mines[x - 1][y + 1].mineNums == -1)
			count++;
	if (y - 1 >= 0)
		if (mines[x][y - 1].mineNums == -1)
			count++;
	if (y + 1 < COL)
		if (mines[x][y + 1].mineNums == -1)
			count++;
	if (x + 1 < ROW && y - 1 >= 0)
		if (mines[x + 1][y - 1].mineNums == -1)
			count++;
	if (x + 1 < ROW)
		if (mines[x + 1][y].mineNums == -1)
			count++;
	if (x + 1 < ROW && y + 1 < COL)
		if (mines[x + 1][y + 1].mineNums == -1)
			count++;
	return count;
}
//---------------

void InitMine()
{
	int i, j;
	//��ͼ��ʼ��Ϊ0
	for (i = 0; i < COL; i++) {
		for (j = 0; j < ROW; j++) {
			mines[i][j].isOpen = false;
			mines[i][j].hasFlag = false;
			mines[i][j].mineNums = 0;
		}
	}
	//����
	srand((unsigned)time(NULL));
	int mine_num = MINENUM;
	while (mine_num) {
		i = rand() % COL;
		j = rand() % ROW;
		if (mines[i][j].mineNums == 0) {//Լ��-1Ϊ��
			mines[i][j].mineNums = -1;
			mine_num--;
		}
	}
	//����ÿ������
	for (i = 0; i < COL; i++) {
		for (j = 0; j < ROW; j++) {
			if (mines[i][j].mineNums != -1)
				mines[i][j].mineNums = SumMine(i, j);
		}
	}
}

void PrintBox(void)
{
	IMAGE blank_space, flag, mine, none;
	IMAGE one, two, three, four, five, six, seven, eight;
	//loadimage(&bar, "IMAGE", "BAR");
	loadimage(&blank_space, "IMAGE", MAKEINTRESOURCE(INIT));
	loadimage(&flag, "IMAGE", MAKEINTRESOURCE(FLAG));
	loadimage(&mine, "IMAGE", MAKEINTRESOURCE(MINE));
	loadimage(&none, "IMAGE", MAKEINTRESOURCE(NONE));
	loadimage(&one, "IMAGE", MAKEINTRESOURCE(NUM1));
	loadimage(&two, "IMAGE", MAKEINTRESOURCE(NUM2));
	loadimage(&three, "IMAGE", MAKEINTRESOURCE(NUM3));
	loadimage(&four, "IMAGE", MAKEINTRESOURCE(NUM4));
	loadimage(&five, "IMAGE", MAKEINTRESOURCE(NUM5));
	loadimage(&six, "IMAGE", MAKEINTRESOURCE(NUM6));
	loadimage(&seven, "IMAGE", MAKEINTRESOURCE(NUM7));
	loadimage(&eight, "IMAGE", MAKEINTRESOURCE(NUM8));
	Coor pos;
	pos.x = 0;
	pos.y = 0;

	//����һ���ĸ߶�
	loadimage(NULL, "IMAGE", MAKEINTRESOURCE(BAR));
	//ɨ����������
	int i, j;
	pos.y += HEIGHT;
	for (i = 0; i < COL; i++) {
		for (j = 0; j < ROW; j++) {
			if (!mines[i][j].isOpen) {
				if (mines[i][j].hasFlag)
					putimage(pos.x, pos.y, &flag);
				else
					putimage(pos.x, pos.y, &blank_space);
			}
			if (mines[i][j].isOpen) {
				switch (mines[i][j].mineNums) {
				case -1: putimage(pos.x, pos.y, &mine);
					break;
				case 0: putimage(pos.x, pos.y, &none);
					break;
				case 1: putimage(pos.x, pos.y, &one);
					break;
				case 2: putimage(pos.x, pos.y, &two);
					break;
				case 3: putimage(pos.x, pos.y, &three);
					break;
				case 4: putimage(pos.x, pos.y, &four);
					break;
				case 5: putimage(pos.x, pos.y, &five);
					break;
				case 6: putimage(pos.x, pos.y, &six);
					break;
				case 7: putimage(pos.x, pos.y, &seven);
					break;
				case 8: putimage(pos.x, pos.y, &eight);
					break;
				}
			}
			pos.x += LEN;
		}
		pos.y += LEN;
		pos.x = 0;
	}
}

Statu Cheak(int i, int j)
{
	j -= HEIGHT;
	//����Ϊ�������
	int x = i / LEN;
	int y = j / LEN;
	if (mines[y][x].isOpen)//�ݹ���,�ų�����ѵ㿪�����
		return -1;//����ֵ
				  //�ǿյ�
	if (mines[y][x].mineNums == 0) {
		firstCheak = 0;
		mines[y][x].isOpen = true;
		if (x - 1 >= 0 && y - 1 >= 0)//û�г�����Ե
			Cheak((x - 1)*LEN, (y - 1)*LEN+HEIGHT);
		if (x - 1 >= 0)
			Cheak((x - 1)*LEN, y*LEN+HEIGHT);
		if (x - 1 >= 0 && y + 1 < COL)
			Cheak((x - 1)*LEN, (y + 1)*LEN+HEIGHT);
		if (y - 1 >= 0)
			Cheak(x*LEN, (y - 1)*LEN + HEIGHT);
		if (y + 1 < COL)
			Cheak(x*LEN, (y + 1)*LEN + HEIGHT);
		if (x + 1 < ROW && y - 1 >= 0)
			Cheak((x + 1)*LEN, (y - 1)*LEN + HEIGHT);
		if (x + 1 < ROW)
			Cheak((x + 1)*LEN, y*LEN + HEIGHT);
		if (x + 1 < ROW && y + 1 < COL)
			Cheak((x + 1)*LEN, (y + 1)*LEN + HEIGHT);
		return SAFE;
	}
	//����
	else if (mines[y][x].mineNums == -1) {
		if (firstCheak) {
			ShowAllMine();//���������׵�����
			return BOMB;
		}
	}
	//������
	else {
		mines[y][x].isOpen = true;
		return SAFE;
	}
	return SAFE;//never happen
}

void ToFlag()
{
	m.y -= HEIGHT;
	//����Ϊ�������
	int i = m.x / LEN;
	int j = m.y / LEN;
	if (mines[j][i].hasFlag == false)
		mines[j][i].hasFlag = true;
	else
		mines[j][i].hasFlag = false;
}

void Lose()
{
	IMAGE lose_board;//150 * 150
	loadimage(&lose_board, "IMAGE", MAKEINTRESOURCE(LOSE));
	putimage(65, 135, &lose_board);
	//���ز˵�
	while (true) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (m.x < 215 && m.x > 185 && m.y < 165 && m.y > 135)
				break;
		}
	}
}

int OpenNum(void)
{
	int count = 0;
	for (int i = 0; i < COL; i++) {
		for (int j = 0; j < ROW; j++) {
			if (mines[i][j].isOpen)
				count++;
		}
	}
	return count;
}

void sortGrade(Grade rank[], int num) {
	for (int i = 0; i < num; i++) {
		for (int j = num-1; j > i; j--) {
			if (rank[j].time < rank[j - 1].time) {
				Grade tmp = rank[j]; rank[j] = rank[j - 1]; rank[j - 1] = tmp;
			}
		}
	}
}

void Win(clock_t time)
{
	//IMAGE win_board;
	//��ӡʱ��
	//while (true) {
	//	GetMouseMsg();
	//�����Ľ���
	char name[10];
	InputBox(name, 10, "��ɨ�ɾ��ˣ�\n�����������(������)");
	//���ַ���д���ļ���ʱ��д���ļ�
	FILE *fp;
	fopen_s(&fp, "grade", "r");
	//�����ļ�(��������¼���������3��
	Grade rank[4];
	int count = 0;
	for (int i = 0; i < 3; i++) {
		//�жϿ��ļ����ʹ��feof�Ļ�����fgetc��freadһ��
		if (EOF == fgetc(fp))//is blank file
			break;
		fseek(fp, -1L, SEEK_CUR);
		
		fread(&rank[i], sizeof(Grade), 1, fp);
		count++;
	}
	fclose(fp);
	//���µĽṹ���ϣ����ߵ���λ
	if (count == 3) {
		strcpy_s(rank[3].name, _countof(rank[3].name), name);
		rank[3].time = (int)time;
	}
	else {
		rank[count].time = (int)time;
		strcpy_s(rank[count].name, _countof(rank[count].name), name);//sizeof(name)/sizeof(name[0])
	}
	//����
	sortGrade(rank, count + 1);
	//д���ļ�
	fopen_s(&fp, "grade", "w");
	for (int i = 0; i < count + 1; i++) {
		fwrite(&rank[i], sizeof(Grade), 1, fp);
	}
	fclose(fp);
	/*old v
	fseek(fp, 0L, SEEK_SET);
	fprintf(fp, "%s                    %d\n", name, (int)time);
	fclose(fp);
	*/
}

void PutScore()
{
	//���ļ��ж�ȡ����
	FILE * fp;
	int y = 180;
	int err;
	err = fopen_s(&fp, "grade", "r");
	if (err != 0) {//δ�ɹ���ȡ
		return;
	}
	//���ݶ���ṹ��
	Grade rank[3];
	int count = 0;
	for (int i = 0; i < 3; i++) {
		if (EOF == fgetc(fp))//is blank file
			break;
		fseek(fp, -1L, SEEK_CUR);

		fread(&rank[i], sizeof(Grade), 1, fp);
		count++;
	}
	fclose(fp);

	settextcolor(RED);
	setbkmode(TRANSPARENT);//͸��
	//��ӡ��ָ��λ��
	char time_s[4];
	for (int i = 0; i < count; i++) {
		outtextxy(70, y, rank[i].name);
		_itoa_s(rank[i].time, time_s, _countof(time_s), 10);
		outtextxy(185, y, time_s);
		y += 35;
	}
	//ÿ���������ֺͷ����ֿ���ӡ

	/***************************old version
	while (fscanf_s(fp, "%[^\n]", line, sizeof(line)) != EOF)
	{
	fgetc(fp);
	outtextxy(70, y, line);
	y += 35;
	if (y == 285)
	break;
	}
	*****************************/
	//for (int i = 0; i < 3; i++, y+=30)
	//	outtextxy(70, y, line);//on board:65,50
	fclose(fp);
}
//------------------------------------------------
int main(void)
{
	StartPage();

	return 0;
}

void StartPage(void)
{
	initgraph(270, 360);
	loadimage(NULL, "IMAGE", MAKEINTRESOURCE(MENU));
	while (true) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (m.x < 180 && m.x > 90 && m.y < 165 && m.y > 135) {
				Game();
				loadimage(NULL, "IMAGE", MAKEINTRESOURCE(MENU));
			}
			else if (m.x < 180 && m.x > 90 && m.y < 210 && m.y > 180) {
				Score();
				loadimage(NULL, "IMAGE", MAKEINTRESOURCE(MENU));
			}
			else if (m.x < 180 && m.x > 90 && m.y < 255 && m.y > 225) {
				closegraph();
				break;
			}
		}
	}
}

void Game(void)
{
	int statu;
	clock_t startime, endtime;
	int initTime = 1;
	InitMine();//��ʼ���������
	PrintBox();//��ӡ�����ͼ
	while (TRUE) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (m.y < HEIGHT)
				continue;
			//������ͼ
			firstCheak = 1;
			statu = Cheak(m.x, m.y);
			if (initTime) {
				startime = clock();
				initTime = 0;
			}
			//��������
			if (statu == SAFE) {
				PrintBox();
			}
			//������
			if (statu == BOMB) {
				PrintBox();
				Lose();//���ز˵�
				break;
			}
			if (OpenNum() == COL * ROW - MINENUM) {
				endtime = clock();
				Win((endtime - startime) / CLOCKS_PER_SEC);//��¼���� ���ز˵�
				break;
			}
		}
		else if (m.uMsg == WM_RBUTTONDOWN) {
			if (m.y < HEIGHT)
				continue;
			ToFlag();
			PrintBox();
		}
	}
}

void Score(void) {
	IMAGE board;//100 * 100
	loadimage(&board, "IMAGE", MAKEINTRESOURCE(GRADE));
	putimage(5, 130, &board);
	PutScore();
	//�жϹرհ�ť
	while (true) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			if (m.x < 265 && m.x > 235 && m.y < 160 && m.y > 130)
				break;
		}
	}
}
