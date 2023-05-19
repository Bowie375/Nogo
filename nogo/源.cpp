#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include<iostream>
#include<math.h>
#include<iomanip>
#include<string>
#include<string.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<deque>
#include<functional>
#include<ctime>
#include<climits>
#include<stdlib.h>
#include<easyx.h>
#include<conio.h>
#include<fstream>
#include<stdlib.h>
#include<climits>
#pragma comment(lib,"Winmm.lib")

using namespace std;

///����ȫ�ֱ���
#define N 9
int chess[N + 2][N + 2];
int chess_air[N + 2][N + 2];
int chesscolor = 1;//��Ϊ1 ��Ϊ2 
int mode = 1;//����Ϊ1 �˻�Ϊ2 ����Ϊ3
int opp_x = 0, opp_y = 0;//��һ�������µ���(����Min_Max()��)

///����������
///����initial()��draw_chess(int,int,int)��reserveBoard()��stepback()��advice()��endgame()
class chessBoard {
public:
	int chess_x0;
	int chess_y0;
	//int chess_color=1;
	void initial();
	void draw_chess(int,int,int);
	void reserveBoard();
	void endgame();
	void stepback();
	void advice();
};
/////��������//////
int length = 1000,width=650;
float Size;
//������
void chessBoard::draw_chess(int x0, int y0,int color) {//������ x0��ʾ���ӵ��� y0��ʾ���ӵ���
	BeginBatchDraw();
	if (color % 2 != 0) {
		setfillcolor(0);
	}
	else setfillcolor(0xffffff);
	solidcircle(x0 * Size, y0 * Size, Size / 4.0);
	EndBatchDraw();
}
//������
void chessBoard::initial() {//��ʼ������
	initgraph(length, width,EX_DBLCLKS);//���ó��ȺͿ��
	Size = (float)width / (N+1);//SizeΪ����С
	setbkcolor(RGB(0xcc, 0x99, 0x0));//���ñ�����ɫ ��ɫ��0xcc ��ɫ��0x99 ��ɫ��0x66 ��ɫ��0x0
	cleardevice();//�������
	BeginBatchDraw();
	setlinecolor(0);
	for (int i = 1; i <= N; i++) {//������
		line(Size, Size * i, width - Size, Size * i);
		line(Size * i, Size, Size * i, width - Size);
	}
	setlinecolor(BLACK);//��������
	for (int i = 0; i < 4; i++) {
		rectangle(700, 50+150*i, 900, 150+150*i);
	}
	settextstyle(70, 15, TEXT("�����п�"));
	if (mode == 2) {
		outtextxy(705, 60, _T("ȷ������"));
	}
	else {
		settextstyle(70, 30, TEXT("�����п�"));
		outtextxy(705, 60, _T("����"));
	}
	settextstyle(70, 15, TEXT("�����п�"));
	outtextxy(705, 215, _T("���ӽ���"));
	settextstyle(70, 30, TEXT("�����п�"));
	outtextxy(705, 360, _T("����"));
	settextstyle(75, 30, TEXT("�����п�"));
	outtextxy(705, 510, _T("�˳�"));
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (i == opp_x && j == opp_y)continue;
			if (chess[i][j] != 0) {
				setfillcolor(chess[i][j] == 1 ? BLACK : WHITE);
				solidcircle(i * Size, j * Size, Size / 3.5);
			}
		}
	}
	if (opp_x != 0 && opp_y != 0) {
		setfillcolor(chesscolor == 1 ? WHITE : BLACK);
		solidcircle(opp_x * Size, opp_y * Size, Size / 4);
		setlinecolor(RED);
		circle(opp_x*Size, opp_y*Size, Size / 3.5);
	}
	EndBatchDraw();
}
//ʵ�ֱ������̹���
void chessBoard::reserveBoard() {
	ofstream fout("prechessboard");//�������һ������֮������
	if (!fout)return;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			fout << chess[i][j] << ' ';
		}
	}
	fout << endl;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			fout << chess_air[i][j] << ' ';
		}
	}
	fout << endl;
	fout << chesscolor;
	fout << endl;
	fout << mode;
	fout << endl;
	fout << opp_x;
	fout << endl;
	fout << opp_y;
	fout.close();

	int tmp[10][10] = {};
	int tmp_air[10][10] = {};
	int tmpcolor = 0;
	if (chesscolor == 1) {
		fout.open("pre1WhiteBoard");//����������һ������֮������
		if (!fout)return;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fout << chess[i][j] << ' ';
			}
		}
		fout << endl;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fout << chess_air[i][j] << ' ';
			}
		}
		fout << endl;
		fout << chesscolor;
		fout << endl;
		fout << mode;
		fout << endl;
		fout << opp_x;
		fout << endl;
		fout << opp_y;
		fout.close();
	}
	else {
		fout.open("pre1BlackBoard");//����������һ������֮������
		if (!fout)return;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fout << chess[i][j] << ' ';
			}
		}
		fout << endl;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fout << chess_air[i][j] << ' ';
			}
		}
		fout << endl;
		fout << chesscolor;
		fout << endl;
		fout << mode;
		fout << endl;
		fout << opp_x;
		fout << endl;
		fout << opp_y;
		fout.close();
	}
}
chessBoard board;

/// ����Ŀ¼��
/// ����display() display_rule() person_person() person_compt() compt_compt() toPreGame()
class Menu {
public:
	void display();
	void display_rule();
	void toPreGame();
	void person_person();
	void person_compt();
	void compt_compt();
};
Menu menu;
//ʵ�ֹ���չʾ
void Menu::display_rule() {
	initgraph(1500, 600);
	IMAGE img;
	loadimage(&img, L"����.jpg", 1500, 600);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);
	settextstyle(70, 50, _T("����"));
	settextcolor(WHITE);
	outtextxy(500, 10, _T("������"));
	settextstyle(30, 17, _T("����"));
	settextcolor(WHITE);
	outtextxy(10, 100, _T("Ŀ�꣺�뷽�跨��Ҫ���Լ����߶Է������ӣ�Χס�������������Լ������ӱ��Է����ߣ�Χס����"));
	outtextxy(10, 140, _T("���̵Ĺ��:��Χ������̴�С��9*9��ע�⣬�����9*9 ָ���Ǹ�����Ŀ�������Ǹ��ӵ�������"));
	outtextxy(10, 180, _T("�����Ǹ��ӵ���������Ϊ����Ҫ���ڸ���ϡ�"));
	outtextxy(10, 220, _T("�����Ⱥ󣺺������֣�˫���������ӣ����Ӻ����Ӳ����ƶ���"));
	outtextxy(10, 260, _T("�и���������Χ��û��ƽ�֡�һ��������������ҽ���һ�����ӿ���"));
	outtextxy(10, 300, _T("ע�����"));
	outtextxy(10, 340, _T("1�����һ�����Ӻ�Χס�˶Է������ӣ���������Ч��"));
	outtextxy(10, 380, _T("2�����Ľ�ֹ��ɱ������������Ч��"));
	outtextxy(10, 420, _T("3�����Ľ�ֹ����(pass)������������Ч��"));
	outtextxy(10, 460, _T("�ر����ѣ�ѡ�����ӽ������Ҫ�����س��������ز���������"));
	outtextxy(10, 500, _T("�밴�س��������ء�>"));
	getchar();
	cleardevice();
	closegraph();
	//menu.display();
}

//ʵ�ָ��̹���
void Menu::toPreGame() {
	ifstream fin("prechessboard");
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			fin >> chess[i][j];
		}
	}
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			fin >> chess_air[i][j];
		}
	}
	fin >> chesscolor;
	fin >> mode;
	fin >> opp_x;
	fin >> opp_y;
	fin.close();
	Menu menu;
	if (mode == 1) {
		menu.person_person();
	}
	else if (mode == 2) {
		menu.person_compt();
	}
	else if (mode == 3) {
		menu.compt_compt();
	}
}
//ʵ��Ŀ¼չʾ
int mnLength = 750, mnWidth = 650;
void Menu::display() {
	initgraph(mnLength, mnWidth, EX_DBLCLKS);
	IMAGE img;
	loadimage(&img, L"�˵�.jpg", mnLength, mnWidth);
	putimage(0, 0, &img);
	BeginBatchDraw();
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(200, 33, _T("�����п�"));
	outtextxy(20, 130, _T("��Χ��"));
	setlinecolor(WHITE);
	rectangle(50, 450, 350, 550);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(70, 60, _T("����"));
	outtextxy(60, 460, _T("����"));
	for (int i = 0; i < 4; i++) {
		setlinecolor(WHITE);
		rectangle(400, 50 + 150 * i, 700, 150 + 150 * i);
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);
		settextstyle(70, 30, _T("����"));
		if (i == 0)outtextxy(410, 50 + 10, _T("���˶�ս"));
		else if (i == 1)outtextxy(410, 50 + 150 * i + 10, _T("�˻���ս"));
		else if (i == 2)outtextxy(410, 50 + 150 * i + 10, _T("������ս"));
		else if (i == 3) {
			settextcolor(WHITE);
			setbkmode(TRANSPARENT);
			settextstyle(70, 60, _T("����"));
			outtextxy(410, 50 + 150 * i + 10, _T("����"));
		}
	}
	EndBatchDraw();
	ExMessage ms;
	while (1) {
		ms = getmessage();
		if (ms.message == WM_LBUTTONDOWN) {
			if (ms.x >= 50 && ms.x <= 350 && ms.y >= 450 && ms.y <= 550) {
				//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
				setlinecolor(RED);
				rectangle(50, 450, 350, 550);
				//Sleep(180);
				cleardevice();
				closegraph();
				menu.display_rule();
				menu.display();
				return;
			}
			else if (ms.x >= 400 && ms.x <= 700 && ms.y >= 500 && ms.y <= 600) {
				//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
				setlinecolor(RED);
				rectangle(400, 500, 700, 600);
				//Sleep(180);
				cleardevice();
				closegraph();
				menu.toPreGame();
				break;
			}
			else {
				chesscolor = 1;
				memset(chess, 0, sizeof(chess));//��ʼ������״̬
				for (int i = 1; i <= N; i++) {//��ʼ����������״̬
					for (int j = 1; j <= N; j++) {
						chess_air[i][j] = 4;
					}
				}
				opp_x = 0; opp_y = 0;
				//��ʼ�������ļ�
				ofstream fout("pre1BlackBoard");
				for (int i = 1; i <= N; i++) {
					for (int j = 1; j <= N; j++) {
						fout << chess[i][j] << ' ';
					}
				}
				fout << endl;
				for (int i = 1; i <= N; i++) {
					for (int j = 1; j <= N; j++) {
						fout << chess_air[i][j] << ' ';
					}
				}
				fout << endl;
				fout << chesscolor;
				fout << endl;
				fout << mode;
				fout.close();
				fout.open("pre1WhiteBoard");
				for (int i = 1; i <= N; i++) {
					for (int j = 1; j <= N; j++) {
						fout << chess[i][j] << ' ';
					}
				}
				fout << endl;
				for (int i = 1; i <= N; i++) {
					for (int j = 1; j <= N; j++) {
						fout << chess_air[i][j] << ' ';
					}
				}
				fout << endl;
				fout << chesscolor;
				fout << endl;
				fout << mode;
				fout.close();
				if (ms.x >= 400 && ms.x <= 700 && ms.y >= 50 && ms.y <= 150) {
					//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
					mode = 1;
					setlinecolor(RED);
					rectangle(400, 50, 700, 150);
					//Sleep(180);
					cleardevice();
					closegraph();
					menu.person_person();
					break;
				}
				else if (ms.x >= 400 && ms.x <= 700 && ms.y >= 200 && ms.y <= 300) {
					//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
					mode = 2;
					setlinecolor(RED);
					rectangle(400, 200, 700, 300);
					//Sleep(180);
					cleardevice();
					closegraph();
					menu.person_compt();
					break;
				}
				else if (ms.x >= 400 && ms.x <= 700 && ms.y >= 350 && ms.y <= 450) {
					//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
					mode = 3;
					setlinecolor(RED);
					rectangle(400, 350, 700, 450);
					//Sleep(180);
					cleardevice();
					closegraph();
					menu.compt_compt();
					break;
				}
			}
		}
	}
}
///ʵ�־���
void caution() {
	board.reserveBoard();
	initgraph(700, 500);
	setbkcolor(WHITE);
	cleardevice();
	setbkmode(TRANSPARENT);
	settextstyle(60, 50, _T("����"));
	settextcolor(RED);
	outtextxy(20, 50, _T("���ﲻ�ܵ㣡"));
	outtextxy(10, 200, _T("��ѡ�������㣡"));
	outtextxy(20, 350, _T("���س�������"));
	getchar();
	cleardevice();
	closegraph();
	menu.toPreGame();
}

//ʵ�ֻ���
void chessBoard::stepback() {
	if (chesscolor == 1) {
		fstream fin;
		fin.open("pre1BlackBoard");
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fin >> chess[i][j];
			}
		}
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fin >> chess_air[i][j];
			}
		}
		fin >> chesscolor;
		fin >> mode;
		fin >> opp_x;
		fin >> opp_y;
		fin.close();
		if (mode == 1) {
			menu.person_person();
		}
		else if (mode == 2) {
			menu.person_compt();
		}
		else if (mode == 3) {
			menu.compt_compt();
		}
		return;
	}
	else {
		fstream fin;
		fin.open("pre1WhiteBoard");//������ʾ���������
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fin >> chess[i][j];  
			}
		}
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				fin >> chess_air[i][j];
			}
		}
		fin >> chesscolor;
		fin >> mode;
		fin >> opp_x;
		fin >> opp_y;
		fin.close();
		if (mode == 1) {
			menu.person_person();
		}
		else if (mode == 2) {
			menu.person_compt();
		}
		else if (mode == 3) {
			menu.compt_compt();
		}
		return;
	}
}

//ʵ���˳�����
void chessBoard::endgame() {
	board.reserveBoard();
	memset(chess, 0, sizeof(chess));//��ʼ������״̬
	for (int i = 1; i <= N; i++) {//��ʼ����������״̬
		for (int j = 1; j <= N; j++) {
			chess_air[i][j] = 4;
		}
	}
	chesscolor = 1;
	mode = 1;
	cleardevice();
	closegraph();
	menu.display();
}


///ʵ����Ӯ����
int d[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
bool bLink[10][10];
bool Link_safe(int x0, int y0, int color) {//����Ƭ������ȫ
	bool flag = 0;
	for (int i = 0; i < 4; i++) {
		int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
		if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1)continue;
		if (chess[x1][y1] == color && bLink[x1][y1] == 0) {
			bLink[x1][y1] = 1;
			if (chess_air[x1][y1] >= 1)return 1;
			flag = Link_safe(x1, y1, color);
			if (flag == 1)break;
		}
	}
	if (flag == 0)return 0;
	else return 1;
}
bool safestep(int x0, int y0, int color) {
	if (x0<1 || x0>N || y0<1 || y0>N)return 0;
	if (chess[x0][y0] != 0)return 0;//�Ѿ����¹�
	//�������ⲽ
	chess[x0][y0] = color;
	for (int i = 0; i < 4; i++) {//������������״̬
		int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
		if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
			chess_air[x0][y0]--;
			continue;
		}
		else if (chess[x1][y1] != 0) {
			chess_air[x1][y1]--;
			chess_air[x0][y0]--;
		}
	}
	//������ɱ������0
	if (chess_air[x0][y0] == 0) {//�ҵ������ְ�Χ�ĵط�
		memset(bLink, 0, sizeof(bLink));
		bLink[x0][y0] = 1;
		if (!Link_safe(x0, y0, color)) {
			chess[x0][y0] = 0;
			for (int i = 0; i < 4; i++) {//������������״̬
				int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
				if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
					chess_air[x0][y0]++;
					continue;
				}
				else if (chess[x1][y1] != 0) {
					chess_air[x1][y1]++;
					chess_air[x0][y0]++;
				}
			}
			return 0;
		}
	}
	//�ж��ܷ���
	for (int i = 0; i < 4; i++) {
		int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
		if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1)continue;
		if (chess[x1][y1] == color % 2 + 1 && chess_air[x1][y1] == 0) {//ֻʣһ����ʱ���������Ƭ����
			memset(bLink, 0, sizeof(bLink));
			bLink[x1][y1] = 1;
			if (!Link_safe(x1, y1, color % 2 + 1)) {//���ܽ����ְ�Χ
				chess[x0][y0] = 0;
				for (int i = 0; i < 4; i++) {//������������״̬
					int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
					if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
						chess_air[x0][y0]++;
						continue;
					}
					else if (chess[x1][y1] != 0) {
						chess_air[x1][y1]++;
						chess_air[x0][y0]++;
					}
				}
				return 0;
			}
		}
		else if (chess[x1][y1] == color && chess_air[x1][y1] == 0) {
			memset(bLink, 0, sizeof(bLink));
			bLink[x1][y1] = 1;
			if (!Link_safe(x1, y1, color)) {
				chess[x0][y0] = 0;
				for (int i = 0; i < 4; i++) {//������������״̬
					int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
					if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
						chess_air[x0][y0]++;
						continue;
					}
					else if (chess[x1][y1] != 0) {
						chess_air[x1][y1]++;
						chess_air[x0][y0]++;
					}
				}
				return 0;
			}
		}
	}
	//����
	chess[x0][y0] = 0;
	for (int i = 0; i < 4; i++) {//������������״̬
		int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
		if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
			chess_air[x0][y0]++;
			continue;
		}
		else if (chess[x1][y1] != 0) {
			chess_air[x1][y1]++;
			chess_air[x0][y0]++;
		}
	}
	return 1;
}
int fail(int color) {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (safestep(i, j, color)) {
				return 0;
			}
		}
	}
	return color % 2 + 1;
}//������1 ������ʤ ������2 ������ʤ

 //��ʾӮ��
void outcome(int tag) {
	initgraph(600, 700);
	IMAGE img;
	cleardevice();
	if (tag == 1) {
		loadimage(&img, L"ʤ����ͼƬ.jpg", 600, 700);
		putimage(0, 0, &img);
		setbkmode(TRANSPARENT);
		settextstyle(70, 50, _T("����"));
		settextcolor(WHITE);
		outtextxy(75, 50, _T("��ϲ�׷���"));
		settextcolor(WHITE);
		settextstyle(60, 30, _T("����"));
		outtextxy(200, 600, _T("���س���������"));
		//PlaySound(L"C:\\lib.wav", 0, SND_SYNC);
		getchar();
		cleardevice();
		closegraph();
		menu.display();
	}
	else {
		loadimage(&img, L"ʤ����ͼƬ.jpg", 600, 700);
		putimage(0, 0, &img);
		setbkmode(TRANSPARENT);
		settextstyle(70, 50, _T("����"));
		settextcolor(WHITE);
		outtextxy(75, 50, _T("��ϲ�ڷ���"));
		settextcolor(WHITE);
		settextstyle(60, 30, _T("����"));
		outtextxy(200, 600, _T("���س���������"));
		//PlaySound(L"C:\\lib.wav", 0, SND_SYNC);
		getchar();
		cleardevice();
		closegraph();
		menu.display();
	}
}


//����㷨
struct availablepoint {//������ߵ�
	int x0;
	int y0;
};
vector<availablepoint>RDS_list;//�������п��ߵ�
void randomSearch() {
	availablepoint point;
	//ö�����п��ߵ�
	for (int x0 = 1; x0 <= N; x0++) {
		for (int y0 = 1; y0 <= N; y0++) {
			if (safestep(x0, y0, chesscolor)) {
				point.x0 = x0;
				point.y0 = y0;
				RDS_list.push_back(point);
			}
		}
	}
	int i = 0;
	i = rand() % RDS_list.size();
	point.x0 = RDS_list[i].x0;
	point.y0 = RDS_list[i].y0;
	if (opp_x != 0 && opp_y != 0) {
		BeginBatchDraw();
		setlinecolor(chesscolor == 1 ? WHITE : BLACK);
		circle(opp_x * Size, opp_y * Size, Size / 3.5);
		setfillcolor(chesscolor == 1 ? WHITE : BLACK);
		solidcircle(opp_x * Size, opp_y * Size, Size / 3.5);
		EndBatchDraw();
	}
	opp_x = point.x0;//����������
	opp_y = point.y0;
	setlinecolor(RED);
	circle(opp_x * Size, opp_y * Size, Size / 3.5);
	board.draw_chess(point.x0, point.y0, chesscolor);//��ʾ������
	/*if (mode != 3)
		PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);*/
	chess[point.x0][point.y0] = chesscolor;//������������״̬
	for (int i = 0; i < 4; i++) {//������������״̬
		int x1 = point.x0 + d[i][0], y1 = point.y0 + d[i][1];
		if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
			chess_air[point.x0][point.y0]--;
			continue;
		}
		if (chess[x1][y1] != 0) {
			chess_air[x1][y1]--;
			chess_air[point.x0][point.y0]--;
		}
	}
	chesscolor = chesscolor % 2 + 1;//�������ӷ�
	board.reserveBoard();//��������״̬
	RDS_list.clear();
}

///min_max�㷨
int depth = 4;//�����������
//��������
int def_x = 0, def_y = 0;
void evaluate_def() {
	bool flag = 0;
	for (int i = 0; i < 4; i++) {
		int x1 = opp_x + d[i][0], y1 = opp_y + d[i][1];
		if (!safestep(x1, y1, chesscolor)) {
			for (int j = 0; j < 4; j++) {
				int x2 = x1 + d[j][0], y2 = x2 + d[j][1];
				if (!safestep(x2, y2, chesscolor))continue;
				chess[x2][y2] = chesscolor;//�������ӵ�״̬
				for (int k = 0; k < 4; k++) {
					int x3 = x2 + d[k][0], y3 = y2 + d[k][1];
					if (x3 == 0 || x3 == N + 1 || y3 == 0 || y3 == N + 1) {
						chess_air[x2][y2]--;
						continue;
					}
					if (chess[x3][y3] != 0) {
						chess_air[x3][y3]--;
						chess_air[x2][y2]--;
					}
				}
				if (!safestep(x1, y1, chesscolor % 2 + 1)) {
					def_x = x2, def_y = y2;
					flag = 1;
				}
				chess[x2][y2] = 0;//�������ӵ�״̬
				for (int k = 0; k < 4; k++) {
					int x3 = x2 + d[k][0], y3 = y2 + d[k][1];
					if (x3 == 0 || x3 == N + 1 || y3 == 0 || y3 == N + 1) {
						chess_air[x2][y2]++;
						continue;
					}
					if (chess[x3][y3] != 0) {
						chess_air[x3][y3]++;
						chess_air[x2][y2]++;
					}
				}
				if (flag)break;
			}
		}
		if (flag)break;
	}
}
int evaluate(int color) {
	int bonus=0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (safestep(i, j, color))bonus++;
			if (safestep(i, j, color % 2 + 1))bonus--;
		}
	}
	return bonus;
}
//�ҳ��������
int land_x = 0, land_y = 0;//������
int alpha[11], beta[11];
int M_M_Search(int x0, int y0, int dpth,int color) {
	if (dpth == 0) {
		if (color == chesscolor)return evaluate(chesscolor);
		else return -evaluate(color);
	}
	vector<availablepoint>safelist;//���ߵ�����
	availablepoint point;
	//������ߵ�
	for (int i = 1; i <= depth; i++) {
		for (int j = -i; j <= i - 1; j++) {
			int x1 = x0 - i;
			int y1 = y0 + j;
			if (x1 < 1)break;
			if (safestep(x1, y1, color)) {
				point.x0 = x1; point.y0 = y1;
				safelist.push_back(point);
			}
		}
		for (int j = -i; j <= i - 1; j++) {
			int x1 = x0 + j;
			int y1 = y0 + i;
			if (y1 > N)break;
			if (safestep(x1, y1, color)) {
				point.x0 = x1; point.y0 = y1;
				safelist.push_back(point);
			}
		}
		for (int j = -i; j <= i - 1; j++) {
			int x1 = x0 + i;
			int y1 = y0 - j;
			if (x1 > N)break;
			if (safestep(x1, y1, color)) {
				point.x0 = x1; point.y0 = y1;
				safelist.push_back(point);
			}
		}
		for (int j = -i; j <= i - 1; j++) {
			int x1 = x0 - j;
			int y1 = y0 - i;
			if (y1 < 1)break;
			if (safestep(x1, y1, color)) {
				point.x0 = x1; point.y0 = y1;
				safelist.push_back(point);
			}
		}
	}
	if (safelist.size() == 0)return -100;//����������Ҳ������ߵ�
	//ģ��֮����̣����ӷ�ʹ�Լ�������󻯣�����ʹ�Լ�������С��
	if (color == chesscolor) {
		int Max = INT_MIN;
		//�����߿��ߵ�ÿ����
		for (int i = 0; i < safelist.size(); i++) {
			int x1 = safelist[i].x0, y1 = safelist[i].y0;
			chess[x1][y1] = color;
			for (int j = 0; j < 4; j++) {//������������״̬
				int x2 = x1 + d[j][0], y2 = y1 + d[j][1];
				if (x2 == 0 || x2 == N + 1 || y2 == 0 || y2 == N + 1) {
					chess_air[x1][y1]--;
					continue;
				}
				if (chess[x2][y2] != 0) {
					chess_air[x2][y2]--;
					chess_air[x1][y1]--;
				}
			}
			int tmp = M_M_Search(x1,y1, dpth - 1, color % 2 + 1);
			evaluate_def();
			if (dpth == depth && x1 == def_x && y1 == def_y) {
				tmp += 10;
			}
			//beta��֦
			if (tmp >beta[dpth]) {
				if (dpth == depth);
				else {
					chess[x1][y1] = 0;//����
					for (int j = 0; j < 4; j++) {
						int x2 = x1 + d[j][0], y2 = y1 + d[j][1];
						if (x2 == 0 || x2 == N + 1 || y2 == 0 || y2 == N + 1) {
							chess_air[x1][y1]++;
							continue;
						}
						if (chess[x2][y2] != 0) {
							chess_air[x2][y2]++;
							chess_air[x1][y1]++;
						}
					}
					return +110;
				}
			}
			if (tmp > Max) {
				Max = tmp;
				if (dpth == depth) {//�ڸ���ȷ�����
					land_x = x1; land_y = y1;
				}
			}
			chess[x1][y1] = 0;//����
			for (int j = 0; j < 4; j++) {
				int x2 = x1 + d[j][0], y2 = y1 + d[j][1];
				if (x2 == 0 || x2 == N + 1 || y2 == 0 || y2 == N + 1) {
					chess_air[x1][y1]++;
					continue;
				}
				if (chess[x2][y2] != 0) {
					chess_air[x2][y2]++;
					chess_air[x1][y1]++;
				}
			}
		}
		beta[dpth] = Max;
		alpha[dpth - 1] = -300;
		return Max;
	}
	else if (color != chesscolor) {
		int Min = INT_MAX;
		for (int i = 0; i < safelist.size(); i++) {
			int x1 = safelist[i].x0, y1 = safelist[i].y0;
			chess[x1][y1] = color;
			for (int j = 0; j < 4; j++) {//������������״̬
				int x2 = x1 + d[j][0], y2 = y1 + d[j][1];
				if (x2 == 0 || x2 == N + 1 || y2 == 0 || y2 == N + 1) {
					chess_air[x1][y1]--;
					continue;
				}
				if (chess[x2][y2] != 0) {
					chess_air[x2][y2]--;
					chess_air[x1][y1]--;
				}
			}
			int tmp = M_M_Search(x1, y1, dpth - 1, color % 2 + 1);
			//alpha��֦
			if (tmp < alpha[dpth]) {
				chess[x1][y1] = 0;//����
				for (int j = 0; j < 4; j++) {//������������״̬
					int x2 = x1 + d[j][0], y2 = y1 + d[j][1];
					if (x2 == 0 || x2 == N + 1 || y2 == 0 || y2 == N + 1) {
						chess_air[x1][y1]++;
						continue;
					}
					if (chess[x2][y2] != 0) {
						chess_air[x2][y2]++;
						chess_air[x1][y1]++;
					}
				}
				return -110;
			}
			if (tmp < Min) {
				Min = tmp;
			}
			chess[x1][y1] = 0;//����
			for (int j = 0; j < 4; j++) {//������������״̬
				int x2 = x1 + d[j][0], y2 = y1 + d[j][1];
				if (x2 == 0 || x2 == N + 1 || y2 == 0 || y2 == N + 1) {
					chess_air[x1][y1]++;
					continue;
				}
				if (chess[x2][y2] != 0) {
					chess_air[x2][y2]++;
					chess_air[x1][y1]++;
				}
			}
		}
		alpha[dpth] = Min;
		beta[dpth - 1] = 300;
		return Min;
	}
}
//Min_Max����ʵ��
void Min_Max() {
	for (int i = 0; i <= 10; i++) {
		alpha[i] = -300;
		beta[i] = 300;
	}
	int D = depth;
	int tmp=M_M_Search(opp_x, opp_y, depth, chesscolor);
	//�Ҳ�������,������������
	if (tmp == -100) {//�ڸ���������Ҳ������ߵ�
		//randomSearch();
		M_M_Search(opp_x, opp_y, D + 1, chesscolor);
	}
	//�ҵ����ŵ�����
	else {
		def_x = 0, def_y = 0;
		if (opp_x != 0 && opp_y != 0) {
			BeginBatchDraw();
			setlinecolor(chesscolor == 1 ? WHITE : BLACK);
			circle(opp_x * Size, opp_y * Size, Size / 3.5);
			setfillcolor(chesscolor == 1 ? WHITE : BLACK);
			solidcircle(opp_x * Size, opp_y * Size, Size / 3.5);
			EndBatchDraw();
		}
		opp_x = land_x;//����������
		opp_y = land_y;
		setlinecolor(RED);
		circle(opp_x * Size, opp_y * Size, Size / 3.5);
		board.draw_chess(opp_x, opp_y, chesscolor);//��ʾ������
		/*if (mode != 3)
			PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);*/
		chess[opp_x][opp_y] = chesscolor;//������������״̬
		for (int i = 0; i < 4; i++) {//������������״̬
			int x1 = opp_x + d[i][0], y1 = opp_y + d[i][1];
			if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
				chess_air[opp_x][opp_y]--;
				continue;
			}
			if (chess[x1][y1] != 0) {
				chess_air[x1][y1]--;
				chess_air[opp_x][opp_y]--;
			}
		}
		chesscolor = chesscolor % 2 + 1;//�������ӷ�
		board.reserveBoard();//��������״̬
	}
}


//ʵ��˫�˶�ս
void Menu::person_person() {
	board.initial();
	ExMessage ms;
	while (!fail(chesscolor)) {
		ms = getmessage();
		if (ms.message == WM_LBUTTONDOWN) {
			if (ms.x >= 700 && ms.x <= 900 && ms.y >= 50 && ms.y <= 150) {
				//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
				cleardevice();
				closegraph();
				menu.display_rule();
				menu.person_person();
				return;
			}
			else if (ms.x >= 700 && ms.x <= 900 && ms.y >= 200 && ms.y <= 300) {
				//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
				cleardevice();
				closegraph();
				initgraph(1000, 350);
				setbkcolor(WHITE);
				cleardevice();
				setbkmode(TRANSPARENT);
				settextstyle(50, 30, _T("����"));
				settextcolor(RED);
				outtextxy(50, 50, _T("�����ĵȴ���ɫ��������ʾ��"));
				outtextxy(50, 130, _T("֮����Ҫ���س�����ȥ������"));
				outtextxy(50, 210, _T("�����밴�س���������"));
				getchar();
				cleardevice();
				closegraph();
				board.initial();
				board.advice();
				getchar();
				board.initial();
			}
			else if (ms.x >= 700 && ms.x <= 900 && ms.y >= 350 && ms.y <= 450) {
				//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
				board.stepback();
				return;
			}
			else if (ms.x >= 700 && ms.x <= 900 && ms.y >= 500 && ms.y <= 600) {
				//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
				board.endgame();
				return;
			}
			else {
				for (int i = 1; i <= N; i++) {//ȷ�����ӵĺ�����
					if (fabs(ms.x - Size * i) < Size / 2) {
						board.chess_x0 = i;
						break;
					}
				}
				for (int i = 1; i <= N; i++) {//ȷ�����ӵ�������
					if (fabs(ms.y - Size * i) < Size / 2) {
						board.chess_y0 = i;
						break;
					}
				}
				if (safestep(board.chess_x0, board.chess_y0, chesscolor)) {
					if (opp_x != 0 && opp_y != 0) {
						BeginBatchDraw();
						setlinecolor(chesscolor==1?WHITE:BLACK);
						circle(opp_x * Size, opp_y * Size, Size / 3.5);
						setfillcolor(chesscolor == 1 ? WHITE : BLACK);
						solidcircle(opp_x * Size, opp_y * Size, Size / 3.5);
						EndBatchDraw();
					}
					setlinecolor(RED);
					circle(board.chess_x0 * Size, board.chess_y0 * Size, Size / 3.5);
					opp_x = board.chess_x0; opp_y = board.chess_y0;
					board.draw_chess(board.chess_x0, board.chess_y0, chesscolor);//��ʾ������
					//PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);
					chess[board.chess_x0][board.chess_y0] = chesscolor;//������������״̬
					for (int i = 0; i < 4; i++) {//������������״̬
						int x1 = board.chess_x0 + d[i][0], y1 = board.chess_y0 + d[i][1];
						if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
							chess_air[board.chess_x0][board.chess_y0]--;
							continue;
						}
						if (chess[x1][y1] != 0) {
							chess_air[x1][y1]--;
							chess_air[board.chess_x0][board.chess_y0]--;
						}
					}
					chesscolor = chesscolor % 2 + 1;//�������ӷ�
					board.reserveBoard();//��������״̬
				}
				else {
					caution();
				}
			}
		}
		flushmessage();
	}
	board.reserveBoard();
	cleardevice();
	closegraph();
	int i = chesscolor;
	outcome(i);
	return;
}

//ʵ���˻���ս
void Menu::person_compt() {
	board.initial();
	ExMessage ms;
	while (!fail(chesscolor)) {
		if (chesscolor == 1) {//�ڷ�Ϊ���
			ms = getmessage();
			if (ms.message == WM_LBUTTONDOWN) {
				if (ms.x >= 700 && ms.x <= 900 && ms.y >= 200 && ms.y <= 300) {
					//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
					cleardevice();
					closegraph();
					initgraph(1000, 350);
					setbkcolor(WHITE);
					cleardevice();
					setbkmode(TRANSPARENT);
					settextstyle(50, 30, _T("����"));
					settextcolor(RED);
					outtextxy(50, 50, _T("�����ĵȴ���ɫ��������ʾ��"));
					outtextxy(50, 130, _T("֮����Ҫ���س�����ȥ������"));
					outtextxy(50, 210, _T("�����밴�س���������"));
					getchar();
					cleardevice();
					closegraph();
					board.initial();
					board.advice();
					getchar();
					board.initial();
				}
				else if (ms.x >= 700 && ms.x <= 900 && ms.y >= 500 && ms.y <= 600) {
					//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
					board.endgame();
					return;
				}
				else {
					for (int i = 1; i <= N; i++) {//ȷ�����ӵĺ�����
						if (fabs(ms.x - Size * i) < Size / 2) {
							board.chess_x0 = i;
							break;
						}
					}
					for (int i = 1; i <= N; i++) {//ȷ�����ӵ�������
						if (fabs(ms.y - Size * i) < Size / 2) {
							board.chess_y0 = i;
							break;
						}
					}
					if (safestep(board.chess_x0, board.chess_y0, chesscolor)) {
						if (opp_x != 0 && opp_y != 0) {
							BeginBatchDraw();
							setlinecolor(chesscolor == 1 ? WHITE : BLACK);
							circle(opp_x * Size, opp_y * Size, Size / 3.5);
							setfillcolor(chesscolor == 1 ? WHITE : BLACK);
							solidcircle(opp_x * Size, opp_y * Size, Size / 3.5);
							EndBatchDraw();
						}
						setlinecolor(RED);
						circle(board.chess_x0 * Size, board.chess_y0 * Size, Size / 3.5);
						board.draw_chess(board.chess_x0, board.chess_y0, chesscolor);//��ʾ������
						//PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);
						chess[board.chess_x0][board.chess_y0] = chesscolor;//������������״̬
						opp_x = board.chess_x0; opp_y = board.chess_y0;
						for (int i = 0; i < 4; i++) {//������������״̬
							int x1 = board.chess_x0 + d[i][0], y1 = board.chess_y0 + d[i][1];
							if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1) {
								chess_air[board.chess_x0][board.chess_y0]--;
								continue;
							}
							if (chess[x1][y1] != 0) {
								chess_air[x1][y1]--;
								chess_air[board.chess_x0][board.chess_y0]--;
							}
						}
						chesscolor = chesscolor % 2 + 1;//�������ӷ�
						board.reserveBoard();//��������״̬
						while (1) {
							ms = getmessage();
							if (ms.message == WM_LBUTTONDOWN) {
								if (ms.x >= 700 && ms.x <= 900 && ms.y >= 50 && ms.y <= 150) { 
									//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
									break; 
								}
								else if (ms.x >= 700 && ms.x <= 900 && ms.y >= 350 && ms.y <= 450) {
									//PlaySound(L"C:\\hint.wav", 0, SND_SYNC);
									board.stepback();
									return;
								}
								break;
							}
						}
					}
					else {
						caution();
					}
				}
			}
			flushmessage();
		}
		else {
			//randomSearch();
			
			Min_Max();
		}
	}
	board.reserveBoard();
	cleardevice();
	closegraph();
	std::system("cls");
	int i = chesscolor;
	outcome(i);
	return;
}

//ʵ�ֻ�����ս
void Menu::compt_compt() {
	srand((unsigned int)time(0));
	board.initial();
	bool flag = 1;
	while (!fail(chesscolor)) {
		if (chesscolor == 1) {//�ڷ�Ϊ�����
			if (flag) {
				randomSearch();
				flag++;
			}
			else Min_Max();
			//randomSearch();
		}
		else {
			//Min_Max();
			randomSearch();
		}
	}
	board.reserveBoard();
	cleardevice();
	closegraph();
	int i = chesscolor;
	outcome(i);
	return;
}

//ʵ�����ӽ���
void chessBoard::advice() {
	M_M_Search(opp_x, opp_y, depth, chesscolor);
	BeginBatchDraw();
	setfillcolor(RED);
	solidcircle(land_x * Size, land_y * Size, Size / 4);
	EndBatchDraw();
}

int main() {
	srand((unsigned int)time(0));
	memset(chess, 0, sizeof(chess));//��ʼ������״̬
	for (int i = 1; i <= N; i++) {//��ʼ����������״̬
		for (int j = 1; j <= N; j++) {
			chess_air[i][j] = 4;
		}
	}
	menu.display();
	return 0;
}