#include"d.h"
int main() {

    initgraph(WIDTH, HEIGHT);
    setbkcolor(BLACK);
    cleardevice();


    IMAGE map;
    loadimage(&map, L"resource\\map.png");
    int mapW = map.getwidth();
    int mapH = map.getheight();

    IMAGE playerImg;
    loadimage(&playerImg, L"resource\\player.png");
    int playerW = playerImg.getwidth();
    int playerH = playerImg.getheight();


    IMAGE bulletImage;
    loadimage(&bulletImage, L"resource\\bullet.png");

    float bulletDX, bulletDY;
    int bulletX = 0, bulletY = 0;
    int t = 0;

    IMAGE enemyImg;
    loadimage(&enemyImg, L"resource\\enemy.png");

    srand(time(0));

    IMAGE start;
    loadimage(&start, L"resource\\begin.png");



    IMAGE end;
    loadimage(&end, L"resource\\end.png");

    ExMessage msg;

    bool isContinue = true;

    while (true)
    {
        putimage(0, 0, &start);
        peekmessage(&msg, EM_MOUSE);
        if (msg.message == WM_LBUTTONDOWN && msg.x >= 500 && msg.x <= 680 && msg.y >= 370 && msg.y <= 450)
        {
            break;
        }
        if (msg.message == WM_LBUTTONDOWN && msg.x >= 500 && msg.x <= 680 && msg.y >= 500 && msg.y <= 590)
        {
            return 0;
        }
        FlushMouseMsgBuffer();
        flushmessage();
        Sleep(30);
    }

    while (isContinue)
    {

        mciSendString(_T("open resource\\music.mp3 alias bkmusic"), NULL, 0, NULL);
        mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);


        int playerspeed = Playerspeed;
        int bulletspeed = Bulletspeed;
        int bulletnumber = Bulletnumber;
        int bulletrange = Bulletrange;
        float HP = constHP;
        int attack = Attack;




        hit = false;
        mark = 0;

        int player_x = mapW / 2 - playerW / 2;
        int player_y = mapH / 2 - playerH / 2;

        ENEMY enemy[Enemynumber];
        for (int i = 0; i < Enemynumber; i++)
        {
            enemy[i].x = (rand() % 7140 + 1530);
            enemy[i].y = (rand() % 4080 + 1020);
            enemy[i].hp = Enemyhp;
            enemy[i].speed = Enemyspeed;
            enemy[i].atk = Enemyatk;
        }


        while (HP > 0)
        {
            if (GetAsyncKeyState('W') & 0x8000)
                player_y -= playerspeed;
            if (GetAsyncKeyState('A') & 0x8000)
                player_x -= playerspeed;
            if (GetAsyncKeyState('S') & 0x8000)
                player_y += playerspeed;
            if (GetAsyncKeyState('D') & 0x8000)
                player_x += playerspeed;

            if (player_y <= 500)
                player_y = 500;
            if (player_x >= 9700)
                player_x = 9700;
            if (player_y >= 5620)
                player_y = 5620;
            if (player_x <= 500)
                player_x = 500;

            peekmessage(&msg, EM_MOUSE);
            if (msg.message == WM_LBUTTONDOWN && bulletnumber > 0)
            {
                bulletnumber--;
                float length = sqrt((msg.x - WIDTH / 2) * (msg.x - WIDTH / 2) + (msg.y - HEIGHT / 2) * (msg.y - HEIGHT / 2));
                bulletDX = (msg.x - WIDTH / 2) / length;
                bulletDY = (msg.y - HEIGHT / 2) / length;
                bulletX = player_x;
                bulletY = player_y;
                t = bulletrange / bulletspeed;
                mciSendString(_T("close atkmusic"), NULL, 0, NULL);
                mciSendString(_T("open resource\\atk.mp3 alias atkmusic"), NULL, 0, NULL);
                mciSendString(_T("play atkmusic"), NULL, 0, NULL);
            }

            for (int i = 0; i < Enemynumber; i++)
            {
                float length = sqrt((enemy[i].x - player_x) * (enemy[i].x - player_x) + (enemy[i].y - player_y) * (enemy[i].y - player_y));
                if (length <= 30)
                {
                    HP -= enemy[i].atk;
                    mciSendString(_T("close bemusic"), NULL, 0, NULL);
                    mciSendString(_T("open resource\\be.mp3 alias bemusic"), NULL, 0, NULL);
                    mciSendString(_T("play bemusic"), NULL, 0, NULL);
                    enemy[i].x = (rand() % 7140 + 1530);
                    enemy[i].y = (rand() % 4080 + 1020);
                }

                if (sqrt((enemy[i].x - bulletX) * (enemy[i].x - bulletX) + (enemy[i].y - bulletY) * (enemy[i].y - bulletY)) <= 30)
                {
                    hit = true;
                    enemy[i].hp -= attack;

                }

                if (enemy[i].hp <= 0)
                {
                    mark += 10;
                    enemy[i].x = (rand() % 7140 + 1530);
                    enemy[i].y = (rand() % 4080 + 1020);
                    enemy[i].hp = Enemyhp + mark / 10;
                    enemy[i].speed = (Enemyspeed + (mark / 80) > Enemyspeedmax ? Enemyspeedmax : Enemyspeed + (mark / 80));
                    enemy[i].atk = ((Enemyatk + ((float)mark / 1000)) > Enemyatkmax ? Enemyatkmax : (Enemyatk + ((float)mark / 1000)));

                }

                enemy[i].dir_x = (enemy[i].x - player_x) / length;
                enemy[i].dir_y = (enemy[i].y - player_y) / length;
                enemy[i].x -= enemy[i].dir_x * enemy[i].speed;
                enemy[i].y -= enemy[i].dir_y * enemy[i].speed;
            }




            CameraPos.x = player_x - WIDTH / 2;
            CameraPos.y = player_y - HEIGHT / 2;

            putimage(0, 0, WIDTH, HEIGHT, &map, CameraPos.x, CameraPos.y);

            drawAlpha(&playerImg, WIDTH / 2 - playerW / 2, HEIGHT / 2 - playerH / 2);

            for (int i = 0; i < Enemynumber; i++)
            {
                if (enemy[i].x < player_x) {
                    loadimage(&enemyImg, L"resource\\enemy.png");
                }
                else {
                    loadimage(&enemyImg, L"resource\\enemy2.png");
                }
                drawenemy(enemy[i].x, enemy[i].y, CameraPos.x, CameraPos.y, &enemyImg);
            }

            if (t > 0 && bulletX >= 1530 && bulletX <= 8670 && bulletY >= 1020 && bulletY <= 5100 && !hit)
            {
                t--;
                drawbullet(bulletX, bulletY, CameraPos.x, CameraPos.y, &bulletImage);
                bulletX += bulletspeed * bulletDX;
                bulletY += bulletspeed * bulletDY;
                hit = false;
            }
            else
            {
                t = -99;
                bulletX = 0;
                bulletY = 0;
                bulletnumber = 1;
                hit = false;
            }

            settextstyle(30, 0, _T("ºÚÌå"));
            setbkmode(TRANSPARENT);
            _stprintf_s(marktext, _T("Score£º%d"), mark);
            _stprintf_s(hptext, _T("HP£º%.2f"), HP);
            _stprintf_s(leveltext, _T("Level£º%d"), level);
            settextcolor(BLACK);
            outtextxy(0, 30, marktext);
            outtextxy(0, 0, hptext);
            outtextxy(0, 60, leveltext);

            peekmessage(&msg, EM_MOUSE);
            if (mark == 100 && level == 1) {
                level++;
                loadimage(&playerImg, L"resource\\player2.png");
                loadimage(&bulletImage, L"resource\\bullet2.png");
            }
            if (mark == 200 && level == 2) {
                level++;
                loadimage(&playerImg, L"resource\\player3.png");
                loadimage(&bulletImage, L"resource\\bullet3.png");
            }

            FlushMouseMsgBuffer();
            flushmessage(-1);

            Sleep(25);
        }

        mciSendString(_T("close bkmusic"), NULL, 0, NULL);
        putimage(0, 0, &end);
        setbkmode(TRANSPARENT);
        settextstyle(50, 0, _T("ºÚÌå"));
        _stprintf_s(marktext, _T("The final score£º%d"), mark);
        outtextxy(400, 300, marktext);
        while (true)
        {
            peekmessage(&msg, EM_MOUSE);
            if (msg.message == WM_LBUTTONDOWN && msg.x >= 430 && msg.x <= 550 && msg.y >= 630 && msg.y <= 690)
            {
                isContinue = false;
                break;
            }
            else if (msg.message == WM_LBUTTONDOWN && msg.x >= 750 && msg.x <= 890 && msg.y >= 630 && msg.y <= 690)
            {
                break;
            }
            FlushMouseMsgBuffer();
            flushmessage();
            Sleep(20);
        }
    }

    closegraph();

    return 0;
}