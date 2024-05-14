#include<iostream>
#include<easyx.h>
#include<graphics.h>
#include<Windows.h>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#pragma comment(lib,"Winmm.lib")

using namespace std;
const int HEIGHT = 750;
const int WIDTH = 1200;

POINT CameraPos;

const int Bulletnumber = 1;
const int Bulletspeed = 30;
const int Bulletrange = 250;
const int Playerspeed = 10;
const float constHP = 3;
const int Attack = 10;
const int Enemynumber = 10;
const int Enemyatk = 1;
const int Enemyatkmax = 3;
const int Enemyspeed = 5;
const int Enemyspeedmax = 20;
const int Enemyhp = 10;
bool hit = false;


int timebl = 0;
int level = 1;
int mark = 0;

TCHAR marktext[20];
TCHAR hptext[15];
TCHAR leveltext[10];

class ENEMY
{
public:

	int x;
	int y;

	float dir_x;
	float dir_y;
	int hp;
	float speed;
	float atk;
};

void drawAlpha(IMAGE* picture, int  picture_x, int picture_y)
{

	DWORD* dst = GetImageBuffer();
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture);
	int picture_width = picture->getwidth();
	int picture_height = picture->getheight();
	int graphWidth = getwidth();
	int graphHeight = getheight();
	int dstX = 0;


	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width;
			int sa = ((src[srcX] & 0xff000000) >> 24);
			int sr = ((src[srcX] & 0xff0000) >> 16);
			int sg = ((src[srcX] & 0xff00) >> 8);
			int sb = src[srcX] & 0xff;
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth;
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
					| (sb * sa / 255 + db * (255 - sa) / 255);
			}
		}
	}
}


void drawbullet(int real_bullet_x, int real_bullet_y, int camera_x, int camera_y, IMAGE* bullet)
{
	int bullet_x = real_bullet_x - camera_x - 15;
	int bullet_y = real_bullet_y - camera_y - 15;
	if (bullet_x >= 0 && bullet_x <= WIDTH - 30 && bullet_y >= 0 && bullet_y <= HEIGHT - 30)
	{
		drawAlpha(bullet, bullet_x, bullet_y);
	}
}


void drawenemy(int real_enemy_x, int real_enemy_y, int camera_x, int camera_y, IMAGE* enemy)
{
	int enemy_x = real_enemy_x - camera_x - 15;
	int enemy_y = real_enemy_y - camera_y - 15;
	if (enemy_x >= 0 && enemy_x <= WIDTH - 60 && enemy_y >= 0 && enemy_y <= HEIGHT - 60)
	{
		drawAlpha(enemy, enemy_x, enemy_y);
	}
}
