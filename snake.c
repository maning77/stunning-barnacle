/*
 * 贪吃蛇（Snake）—— Windows 控制台版
 * 操作说明：W/S/A/D 控制方向，X 退出
 *
 * 【修改记录】
 * 2026-09-07 v1 新增：① 文件头说明与修改记录注释
 *                   ② 修复每次食物位置相同的问题（添加随机种子 srand）
 * 2026-09-07 v2 新增：P 键暂停 / 继续游戏
 */
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20

// 蛇身体坐标
int snakeX[100], snakeY[100];
int len = 3;
int foodX, foodY;
int dir = 2; // 1上 2右 3下 4左
int gameOver = 0;
int score = 0;
int paused = 0;   // 暂停状态：1=暂停中 0=运行

// 设置光标位置
void gotoxy(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// 隐藏光标
void hideCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// 生成食物
void createFood() {
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
}

// 初始化游戏
void init() {
    gameOver = 0;
    dir = 2;
    len = 3;
    score = 0;
    paused = 0;
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;
    snakeX[1] = WIDTH / 2 - 1;
    snakeY[1] = HEIGHT / 2;
    snakeX[2] = WIDTH / 2 - 2;
    snakeY[2] = HEIGHT / 2;
    srand((unsigned)time(NULL)); // 设置随机种子，保证每次食物位置不同
    createFood();
    hideCursor();
}

// 绘制画面
void draw() {
    gotoxy(0,0);
    // 上边框
    for(int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");

    for(int y = 1; y < HEIGHT - 1; y++) {
        for(int x = 0; x < WIDTH; x++) {
            if(x == 0 || x == WIDTH -1) {
                printf("#");
            } else if(x == snakeX[0] && y == snakeY[0]) {
                printf("O"); //蛇头
            } else {
                int isBody = 0;
                for(int i = 1; i < len; i++) {
                    if(snakeX[i]==x && snakeY[i]==y) {
                        printf("o");//蛇身体
                        isBody = 1;
                        break;
                    }
                }
                if(!isBody) {
                    if(x == foodX && y == foodY) {
                        printf("F");
                    } else {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }
    //下边框
    for(int i = 0; i < WIDTH; i++) printf("#");
    printf("\n");
    if(paused)
        printf("分数：%d   [已暂停] 按 P 继续游戏   X退出\n", score);
    else
        printf("分数：%d  方向：W上 S下 A左 D右  P暂停 X退出\n", score);
}

// 键盘输入
void input() {
    if(_kbhit()) {
        switch(_getch()) {
            case 'w': if(dir !=3) dir = 1; break;
            case 's': if(dir !=1) dir = 3; break;
            case 'a': if(dir !=2) dir = 4; break;
            case 'd': if(dir !=4) dir = 2; break;
            case 'p': paused = !paused; break; // P 暂停/继续
            case 'x': gameOver = 1; break;
        }
    }
}

// 蛇逻辑更新
void logic() {
    if(paused) return;   // 暂停中不更新游戏逻辑

    //身体跟随
    int preX = snakeX[0];
    int preY = snakeY[0];
    int tempX, tempY;
    snakeX[0] += (dir==1?-1:(dir==3?1:0));
    snakeY[0] += (dir==4?-1:(dir==2?1:0));

    for(int i = 1; i < len; i++) {
        tempX = snakeX[i];
        tempY = snakeY[i];
        snakeX[i] = preX;
        snakeY[i] = preY;
        preX = tempX;
        preY = tempY;
    }

    //撞墙
    if(snakeX[0]<=0 || snakeX[0]>=WIDTH-1 || snakeY[0]<=0 || snakeY[0]>=HEIGHT-1) {
        gameOver = 1;
    }
    //撞到自己
    for(int i = 1; i < len; i++) {
        if(snakeX[0]==snakeX[i] && snakeY[0]==snakeY[i]) {
            gameOver = 1;
        }
    }
    //吃到食物
    if(snakeX[0]==foodX && snakeY[0]==foodY) {
        score += 10;
        len++;
        createFood();
    }
}

int main() {
    init();
    while(!gameOver) {
        draw();
        input();
        logic();
        Sleep(120); // 控制速度
    }
    gotoxy(0, HEIGHT+2);
    printf("游戏结束！最终分数：%d\n", score);
    return 0;
}
