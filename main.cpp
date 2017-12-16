//基于EasyX头文件开发的小游戏--Jimmy开发
//在Visual Studio 2017 环境下编译运行成功通过测试
#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <WinUser.h>

typedef struct position {
	int x;
	int y;
}posType;//坐标结构体

#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000)

void init(void);//初始化界面
void gamebegin(void);//游戏开始界面
void gameplay(void);//玩游戏的具体过程
void close(void);//释放资源
void drawCursor(posType pos, COLORREF cl);//重绘光标
void drawTime(int sec);//重绘时间
void drawScore(int score);//重绘分数
void getSameColorBalls(posType cur, COLORREF cl);//获得相同颜色小球
int isValid(posType cur, COLORREF cl);//判断小球颜色是否相同
void ballsfall(void);//小球下落
void turn(void);//小球排序

//颜色全局数组
COLORREF colorArr[6] = { RGB(255,0,0) ,RGB(0,255,0) ,RGB(0,0,255) ,RGB(255,255,0) ,RGB(255,0,255) ,RGB(0,255,255) };

//当前光标位置全局变量
posType cur;

//同色小球坐标数组
posType ballsArr[180];

//同色小球个数
int index;

int main(void) {
	init();
	gamebegin();
	gameplay();
	close();

	return 0;
}//main

void init() {
	initgraph(1000, 700);
}//init

void gamebegin() {
	//绘制矩形边框
	setlinecolor(RGB(50, 50, 50));
	setlinestyle(PS_SOLID, 10);
	rectangle(255, 45, 745, 655);//根据放置小球的数目以及线宽计算得出的坐标

	//绘制小球,小球半径是20
	 //setlinecolor(RGB(255, 0, 0));
	 //setfillcolor(RGB(255, 0, 0));
	setlinestyle(PS_SOLID);
	srand((unsigned)time(NULL));//提供随机数种子
	for (int i = 280; i < 740; i += 40) {
		for (int j = 70; j < 650; j += 40) {
			COLORREF cl = colorArr[rand()% 6];
			setlinecolor(cl);
			setfillcolor(cl);
			fillcircle(i, j, 15);
		}
	}
	//绘制光标
	cur.x = 480;//光标初始位置
	cur.y = 390;
	drawCursor(cur, RGB(255, 255, 255));

	//绘制游戏时间
	drawTime(30);

	//绘制游戏得分
	drawScore(0);

}//gamebegin

void gameplay() {
	int score = 0;
	for (int i = 300; i > -1; i--) {
		if (i % 10 == 0) {
			drawTime(i / 10);
		}
		if (KEY_DOWN(VK_UP)&&cur.y>70) {
			drawCursor(cur, RGB(0, 0, 0));
			cur.y -= 40;
			drawCursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_DOWN)&&cur.y<630) {
			drawCursor(cur, RGB(0, 0, 0));
			cur.y += 40;
			drawCursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_LEFT)&&cur.x>280) {
			drawCursor(cur, RGB(0, 0, 0));
			cur.x -= 40;
			drawCursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_RIGHT)&&cur.x<720) {
			drawCursor(cur, RGB(0, 0, 0));
			cur.x += 40;
			drawCursor(cur, RGB(255, 255, 255));
		}
		else if (KEY_DOWN(VK_RETURN)|| KEY_DOWN(VK_SPACE)) {
			//获得光标所在位置的相同颜色的小球坐标，存入数组，记录个数
			getSameColorBalls(cur,getpixel(cur.x, cur.y));

			//将同色小球消失一段时间，即让数组中元素变黑一段时间
			if (index > 1) {
				for (int k = 0; k < index; k++) {
					setlinecolor(RGB(0, 0, 0));
					setfillcolor(RGB(0, 0, 0));
					fillcircle(ballsArr[k].x, ballsArr[k].y, 15);
				}
				Sleep(300);
				//上方小球掉落补位
				ballsfall();

				//刷新分数
				score += index;
				drawScore(score);


			}
			index = 0;//重置相同小球数目
			
		}
		Sleep(100);//延迟100ms
	}
	//游戏结束
	cleardevice();
	TCHAR str[30];
	settextcolor(RGB(255, 255, 0));
	settextstyle(50, 0, _T("微软雅黑"));
	_stprintf_s(str, _T("游戏结束"));
	outtextxy(425, 200, str);
	drawScore(score);
}//gameplay

void close() {
	getchar();
	closegraph();
}//close

void drawCursor(posType pos, COLORREF cl) {
	setlinecolor(cl);
	rectangle(pos.x - 20, pos.y - 20, pos.x + 20, pos.y + 20);
}//drawCursor

void drawTime(int sec) {
	TCHAR str[30];
	settextcolor(RGB(255, 255, 0));
	settextstyle(25, 0, _T("微软雅黑"));
	_stprintf_s(str, _T("剩余时间：%2d s"), sec);
	outtextxy(20, 50, str);
}//drawTime

void drawScore(int score) {
	TCHAR str[30];
	settextcolor(RGB(255, 0, 0));
	settextstyle(25, 0, _T("微软雅黑"));
	_stprintf_s(str, _T("分数：%d"), score);
	outtextxy(20, 600, str);
}//drawScore

void getSameColorBalls(posType cur, COLORREF cl) {
	ballsArr[index].x = cur.x;
	ballsArr[index].y = cur.y;
	index++;

	posType tmpPos;
	for (int k = 0; k < 4; k++) {
		switch (k) {
			//0,1,2,3分别代表上下左右
			case 0:
				tmpPos.x = cur.x;
				tmpPos.y = cur.y - 40;
				break;
			case 1:
				tmpPos.x = cur.x;
				tmpPos.y = cur.y + 40;
				break;
			case 2:
				tmpPos.x = cur.x - 40;
				tmpPos.y = cur.y;
				break;
			case 3:
				tmpPos.x = cur.x + 40;
				tmpPos.y = cur.y;
				break;

		}

		if (isValid(tmpPos, cl)) {
					
			getSameColorBalls(tmpPos, cl);//递归继续寻找相邻相同颜色小球
		}
	}

}//getSameColorBalls

int isValid(posType cur, COLORREF cl) {
	if (getpixel(cur.x, cur.y) != cl) {
		return 0;
	}
	else {
		//判断小球是否已经在数组中存在
		for (int i = 0; i < index; i++) {
			if (cur.x == ballsArr[i].x&&cur.y == ballsArr[i].y) {
				return 0;
			}
		}
		return 1;
	}
}

void turn() {
	int i, j;
	posType temp;
	for(j=0;j<index-1;j++)
		for (i = 0; i < index - 1; i++) {
			if (ballsArr[i].x > ballsArr[i + 1].x) {
				temp = ballsArr[i];
				ballsArr[i] = ballsArr[i + 1];
				ballsArr[i + 1] = temp;
			}

			if (ballsArr[i].y > ballsArr[i + 1].y) {
				temp = ballsArr[i];
				ballsArr[i] = ballsArr[i + 1];
				ballsArr[i + 1] = temp;
			}
		}

}

void ballsfall() {
	turn();
	for (int i = 0; i < index; i++) {
		for (int k = ballsArr[i].y; k > 70;k -= 40) {
			COLORREF cl = getpixel(ballsArr[i].x, k - 40);
			setlinecolor(cl);
			setfillcolor(cl);
			fillcircle(ballsArr[i].x, k, 15);
		}

		COLORREF cl = colorArr[rand() % 6];
		setlinecolor(cl);
		setfillcolor(cl);
		fillcircle(ballsArr[i].x, 70, 15);
	}
}

