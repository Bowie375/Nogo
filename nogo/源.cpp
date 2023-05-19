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

///定义全局变量
#define N 9
int chess[N + 2][N + 2];
int chess_air[N + 2][N + 2];
int chesscolor = 1;//黑为1 白为2 
int mode = 1;//人人为1 人机为2 机机为3
int opp_x = 0, opp_y = 0;//上一步对手下的棋(用于Min_Max()中)

///建立棋盘类
///包含initial()、draw_chess(int,int,int)、reserveBoard()、stepback()、advice()、endgame()
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
/////建立棋盘//////
int length = 1000,width=650;
float Size;
//画棋子
void chessBoard::draw_chess(int x0, int y0,int color) {//画棋子 x0表示下子的列 y0表示下子的行
	BeginBatchDraw();
	if (color % 2 != 0) {
		setfillcolor(0);
	}
	else setfillcolor(0xffffff);
	solidcircle(x0 * Size, y0 * Size, Size / 4.0);
	EndBatchDraw();
}
//画棋盘
void chessBoard::initial() {//初始化棋盘
	initgraph(length, width,EX_DBLCLKS);//设置长度和宽度
	Size = (float)width / (N+1);//Size为棋格大小
	setbkcolor(RGB(0xcc, 0x99, 0x0));//设置背景颜色 红色是0xcc 绿色是0x99 蓝色是0x66 黑色是0x0
	cleardevice();//背景填充
	BeginBatchDraw();
	setlinecolor(0);
	for (int i = 1; i <= N; i++) {//画网格
		line(Size, Size * i, width - Size, Size * i);
		line(Size * i, Size, Size * i, width - Size);
	}
	setlinecolor(BLACK);//画工具栏
	for (int i = 0; i < 4; i++) {
		rectangle(700, 50+150*i, 900, 150+150*i);
	}
	settextstyle(70, 15, TEXT("华文行楷"));
	if (mode == 2) {
		outtextxy(705, 60, _T("确定落子"));
	}
	else {
		settextstyle(70, 30, TEXT("华文行楷"));
		outtextxy(705, 60, _T("帮助"));
	}
	settextstyle(70, 15, TEXT("华文行楷"));
	outtextxy(705, 215, _T("落子建议"));
	settextstyle(70, 30, TEXT("华文行楷"));
	outtextxy(705, 360, _T("悔棋"));
	settextstyle(75, 30, TEXT("华文行楷"));
	outtextxy(705, 510, _T("退出"));
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
//实现保存棋盘功能
void chessBoard::reserveBoard() {
	ofstream fout("prechessboard");//储存最后一步走完之后的棋局
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
		fout.open("pre1WhiteBoard");//储存白棋最后一步走完之后的棋局
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
		fout.open("pre1BlackBoard");//储存黑棋最后一步走完之后的棋局
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

/// 建立目录类
/// 包含display() display_rule() person_person() person_compt() compt_compt() toPreGame()
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
//实现规则展示
void Menu::display_rule() {
	initgraph(1500, 600);
	IMAGE img;
	loadimage(&img, L"规则.jpg", 1500, 600);
	putimage(0, 0, &img);
	setbkmode(TRANSPARENT);
	settextstyle(70, 50, _T("楷体"));
	settextcolor(WHITE);
	outtextxy(500, 10, _T("规则简介"));
	settextstyle(30, 17, _T("楷体"));
	settextcolor(WHITE);
	outtextxy(10, 100, _T("目标：想方设法不要让自己提走对方的棋子（围住），尽可能让自己的棋子被对方提走（围住）。"));
	outtextxy(10, 140, _T("棋盘的规格:不围棋的棋盘大小是9*9。注意，这里的9*9 指的是格点的数目，并不是格子的数量，"));
	outtextxy(10, 180, _T("并不是格子的数量，因为棋子要下在格点上。"));
	outtextxy(10, 220, _T("落子先后：黑子先手，双方轮流落子，落子后棋子不可移动。"));
	outtextxy(10, 260, _T("判负条件：不围棋没有平局。一方输掉比赛，当且仅当一方无子可下"));
	outtextxy(10, 300, _T("注意事项："));
	outtextxy(10, 340, _T("1）如果一方落子后围住了对方的棋子，则落子无效；"));
	outtextxy(10, 380, _T("2）对弈禁止自杀，否则落子无效；"));
	outtextxy(10, 420, _T("3）对弈禁止空手(pass)，否则落子无效。"));
	outtextxy(10, 460, _T("特别提醒：选择落子建议后需要单击回车键来返回操作！！！"));
	outtextxy(10, 500, _T("请按回车键来返回—>"));
	getchar();
	cleardevice();
	closegraph();
	//menu.display();
}

//实现复盘功能
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
//实现目录展示
int mnLength = 750, mnWidth = 650;
void Menu::display() {
	initgraph(mnLength, mnWidth, EX_DBLCLKS);
	IMAGE img;
	loadimage(&img, L"菜单.jpg", mnLength, mnWidth);
	putimage(0, 0, &img);
	BeginBatchDraw();
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(200, 33, _T("华文行楷"));
	outtextxy(20, 130, _T("不围棋"));
	setlinecolor(WHITE);
	rectangle(50, 450, 350, 550);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(70, 60, _T("楷体"));
	outtextxy(60, 460, _T("规则"));
	for (int i = 0; i < 4; i++) {
		setlinecolor(WHITE);
		rectangle(400, 50 + 150 * i, 700, 150 + 150 * i);
		settextcolor(WHITE);
		setbkmode(TRANSPARENT);
		settextstyle(70, 30, _T("楷体"));
		if (i == 0)outtextxy(410, 50 + 10, _T("人人对战"));
		else if (i == 1)outtextxy(410, 50 + 150 * i + 10, _T("人机对战"));
		else if (i == 2)outtextxy(410, 50 + 150 * i + 10, _T("机机对战"));
		else if (i == 3) {
			settextcolor(WHITE);
			setbkmode(TRANSPARENT);
			settextstyle(70, 60, _T("楷体"));
			outtextxy(410, 50 + 150 * i + 10, _T("复盘"));
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
				memset(chess, 0, sizeof(chess));//初始化棋子状态
				for (int i = 1; i <= N; i++) {//初始化棋子气的状态
					for (int j = 1; j <= N; j++) {
						chess_air[i][j] = 4;
					}
				}
				opp_x = 0; opp_y = 0;
				//初始化储存文件
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
///实现警告
void caution() {
	board.reserveBoard();
	initgraph(700, 500);
	setbkcolor(WHITE);
	cleardevice();
	setbkmode(TRANSPARENT);
	settextstyle(60, 50, _T("楷体"));
	settextcolor(RED);
	outtextxy(20, 50, _T("这里不能点！"));
	outtextxy(10, 200, _T("请选择其它点！"));
	outtextxy(20, 350, _T("按回车键继续"));
	getchar();
	cleardevice();
	closegraph();
	menu.toPreGame();
}

//实现悔棋
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
		fin.open("pre1WhiteBoard");//重新显示悔棋后的棋局
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

//实现退出功能
void chessBoard::endgame() {
	board.reserveBoard();
	memset(chess, 0, sizeof(chess));//初始化棋子状态
	for (int i = 1; i <= N; i++) {//初始化棋子气的状态
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


///实现判赢函数
int d[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
bool bLink[10][10];
bool Link_safe(int x0, int y0, int color) {//连成片的气安全
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
	if (chess[x0][y0] != 0)return 0;//已经被下过
	//尝试走这步
	chess[x0][y0] = color;
	for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
	//不能自杀，返回0
	if (chess_air[x0][y0] == 0) {//找到被对手包围的地方
		memset(bLink, 0, sizeof(bLink));
		bLink[x0][y0] = 1;
		if (!Link_safe(x0, y0, color)) {
			chess[x0][y0] = 0;
			for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
	//判断能否下
	for (int i = 0; i < 4; i++) {
		int x1 = x0 + d[i][0], y1 = y0 + d[i][1];
		if (x1 == 0 || x1 == N + 1 || y1 == 0 || y1 == N + 1)continue;
		if (chess[x1][y1] == color % 2 + 1 && chess_air[x1][y1] == 0) {//只剩一口气时，检查连成片的气
			memset(bLink, 0, sizeof(bLink));
			bLink[x1][y1] = 1;
			if (!Link_safe(x1, y1, color % 2 + 1)) {//不能将对手包围
				chess[x0][y0] = 0;
				for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
				for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
	//回溯
	chess[x0][y0] = 0;
	for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
}//若返回1 则黑棋获胜 若返回2 则白棋获胜

 //显示赢家
void outcome(int tag) {
	initgraph(600, 700);
	IMAGE img;
	cleardevice();
	if (tag == 1) {
		loadimage(&img, L"胜利者图片.jpg", 600, 700);
		putimage(0, 0, &img);
		setbkmode(TRANSPARENT);
		settextstyle(70, 50, _T("楷体"));
		settextcolor(WHITE);
		outtextxy(75, 50, _T("恭喜白方！"));
		settextcolor(WHITE);
		settextstyle(60, 30, _T("楷体"));
		outtextxy(200, 600, _T("按回车键继续！"));
		//PlaySound(L"C:\\lib.wav", 0, SND_SYNC);
		getchar();
		cleardevice();
		closegraph();
		menu.display();
	}
	else {
		loadimage(&img, L"胜利者图片.jpg", 600, 700);
		putimage(0, 0, &img);
		setbkmode(TRANSPARENT);
		settextstyle(70, 50, _T("楷体"));
		settextcolor(WHITE);
		outtextxy(75, 50, _T("恭喜黑方！"));
		settextcolor(WHITE);
		settextstyle(60, 30, _T("楷体"));
		outtextxy(200, 600, _T("按回车键继续！"));
		//PlaySound(L"C:\\lib.wav", 0, SND_SYNC);
		getchar();
		cleardevice();
		closegraph();
		menu.display();
	}
}


//随机算法
struct availablepoint {//构造可走点
	int x0;
	int y0;
};
vector<availablepoint>RDS_list;//储存所有可走点
void randomSearch() {
	availablepoint point;
	//枚举所有可走点
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
	opp_x = point.x0;//储存上手棋
	opp_y = point.y0;
	setlinecolor(RED);
	circle(opp_x * Size, opp_y * Size, Size / 3.5);
	board.draw_chess(point.x0, point.y0, chesscolor);//显示落棋结果
	/*if (mode != 3)
		PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);*/
	chess[point.x0][point.y0] = chesscolor;//更新棋盘数组状态
	for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
	chesscolor = chesscolor % 2 + 1;//更换落子方
	board.reserveBoard();//保存棋盘状态
	RDS_list.clear();
}

///min_max算法
int depth = 4;//定义搜索深度
//评估函数
int def_x = 0, def_y = 0;
void evaluate_def() {
	bool flag = 0;
	for (int i = 0; i < 4; i++) {
		int x1 = opp_x + d[i][0], y1 = opp_y + d[i][1];
		if (!safestep(x1, y1, chesscolor)) {
			for (int j = 0; j < 4; j++) {
				int x2 = x1 + d[j][0], y2 = x2 + d[j][1];
				if (!safestep(x2, y2, chesscolor))continue;
				chess[x2][y2] = chesscolor;//更新棋子的状态
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
				chess[x2][y2] = 0;//更新棋子的状态
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
//找出最大收益
int land_x = 0, land_y = 0;//最佳落点
int alpha[11], beta[11];
int M_M_Search(int x0, int y0, int dpth,int color) {
	if (dpth == 0) {
		if (color == chesscolor)return evaluate(chesscolor);
		else return -evaluate(color);
	}
	vector<availablepoint>safelist;//可走点数列
	availablepoint point;
	//储存可走点
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
	if (safelist.size() == 0)return -100;//给定深度内找不到可走点
	//模拟之后过程，落子方使自己收益最大化，对手使自己收益最小化
	if (color == chesscolor) {
		int Max = INT_MIN;
		//尝试走可走的每个点
		for (int i = 0; i < safelist.size(); i++) {
			int x1 = safelist[i].x0, y1 = safelist[i].y0;
			chess[x1][y1] = color;
			for (int j = 0; j < 4; j++) {//更新棋子气的状态
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
			//beta剪枝
			if (tmp >beta[dpth]) {
				if (dpth == depth);
				else {
					chess[x1][y1] = 0;//回溯
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
				if (dpth == depth) {//在根部确定落点
					land_x = x1; land_y = y1;
				}
			}
			chess[x1][y1] = 0;//回溯
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
			for (int j = 0; j < 4; j++) {//更新棋子气的状态
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
			//alpha剪枝
			if (tmp < alpha[dpth]) {
				chess[x1][y1] = 0;//回溯
				for (int j = 0; j < 4; j++) {//更新棋子气的状态
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
			chess[x1][y1] = 0;//回溯
			for (int j = 0; j < 4; j++) {//更新棋子气的状态
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
//Min_Max函数实现
void Min_Max() {
	for (int i = 0; i <= 10; i++) {
		alpha[i] = -300;
		beta[i] = 300;
	}
	int D = depth;
	int tmp=M_M_Search(opp_x, opp_y, depth, chesscolor);
	//找不到可走,增加搜索层数
	if (tmp == -100) {//在给定深度内找不到可走点
		//randomSearch();
		M_M_Search(opp_x, opp_y, D + 1, chesscolor);
	}
	//找到最优点落子
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
		opp_x = land_x;//储存上手棋
		opp_y = land_y;
		setlinecolor(RED);
		circle(opp_x * Size, opp_y * Size, Size / 3.5);
		board.draw_chess(opp_x, opp_y, chesscolor);//显示落棋结果
		/*if (mode != 3)
			PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);*/
		chess[opp_x][opp_y] = chesscolor;//更新棋盘数组状态
		for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
		chesscolor = chesscolor % 2 + 1;//更换落子方
		board.reserveBoard();//保存棋盘状态
	}
}


//实现双人对战
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
				settextstyle(50, 30, _T("楷体"));
				settextcolor(RED);
				outtextxy(50, 50, _T("请耐心等待红色建议子显示，"));
				outtextxy(50, 130, _T("之后需要按回车键撤去建议子"));
				outtextxy(50, 210, _T("现在请按回车键继续！"));
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
				for (int i = 1; i <= N; i++) {//确定落子的横坐标
					if (fabs(ms.x - Size * i) < Size / 2) {
						board.chess_x0 = i;
						break;
					}
				}
				for (int i = 1; i <= N; i++) {//确定落子的纵坐标
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
					board.draw_chess(board.chess_x0, board.chess_y0, chesscolor);//显示落棋结果
					//PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);
					chess[board.chess_x0][board.chess_y0] = chesscolor;//更新棋盘数组状态
					for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
					chesscolor = chesscolor % 2 + 1;//更换落子方
					board.reserveBoard();//保存棋盘状态
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

//实现人机对战
void Menu::person_compt() {
	board.initial();
	ExMessage ms;
	while (!fail(chesscolor)) {
		if (chesscolor == 1) {//黑方为玩家
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
					settextstyle(50, 30, _T("楷体"));
					settextcolor(RED);
					outtextxy(50, 50, _T("请耐心等待红色建议子显示，"));
					outtextxy(50, 130, _T("之后需要按回车键撤去建议子"));
					outtextxy(50, 210, _T("现在请按回车键继续！"));
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
					for (int i = 1; i <= N; i++) {//确定落子的横坐标
						if (fabs(ms.x - Size * i) < Size / 2) {
							board.chess_x0 = i;
							break;
						}
					}
					for (int i = 1; i <= N; i++) {//确定落子的纵坐标
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
						board.draw_chess(board.chess_x0, board.chess_y0, chesscolor);//显示落棋结果
						//PlaySound(L"C:\\Yes.wav", 0, SND_SYNC);
						chess[board.chess_x0][board.chess_y0] = chesscolor;//更新棋盘数组状态
						opp_x = board.chess_x0; opp_y = board.chess_y0;
						for (int i = 0; i < 4; i++) {//更新棋子气的状态
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
						chesscolor = chesscolor % 2 + 1;//更换落子方
						board.reserveBoard();//保存棋盘状态
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

//实现机机对战
void Menu::compt_compt() {
	srand((unsigned int)time(0));
	board.initial();
	bool flag = 1;
	while (!fail(chesscolor)) {
		if (chesscolor == 1) {//黑方为随机数
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

//实现落子建议
void chessBoard::advice() {
	M_M_Search(opp_x, opp_y, depth, chesscolor);
	BeginBatchDraw();
	setfillcolor(RED);
	solidcircle(land_x * Size, land_y * Size, Size / 4);
	EndBatchDraw();
}

int main() {
	srand((unsigned int)time(0));
	memset(chess, 0, sizeof(chess));//初始化棋子状态
	for (int i = 1; i <= N; i++) {//初始化棋子气的状态
		for (int j = 1; j <= N; j++) {
			chess_air[i][j] = 4;
		}
	}
	menu.display();
	return 0;
}