#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

//#define DEBUG

typedef struct snakeNode {
    int x;
    int y;
    struct snakeNode *next;
}SNAKENODE;

typedef struct snake {
    SNAKENODE *head;
    SNAKENODE *tail;
    int snakeLength;
}SNAKE;

typedef struct {
    int x;
    int y;
}SEED;

enum key {
    W_UP    = 119,
    A_LEFT  = 97,
    S_DOWN  = 115,
    D_RIGHT = 100,
    Q_QUIT  = 81,
};

int windowsX = 100,windowsY = 28;
int seedColor = 31,unSeedColor = 37;
char snakeNodeTag[5]      = "⬜";
char clearSnakeNodeTag[5] = "  ";
char direction = 'w';
SEED seed;

char *FormatSeedContent(int color,char seedContent[])
{
    sprintf(seedContent,"\33[%d;%dH\33[%dm%s\33[0m",seed.y,seed.x,color,snakeNodeTag);
    return seedContent;
}

void DispSeed(int color)
{
    char seedContent[30];
    printf("%s",FormatSeedContent(color,seedContent));
}

void InitSeed()
{
    srand((unsigned)time(NULL));
    seed.x = (rand() % 96)/2 + 2;
    seed.x += (seed.x % 2);
    seed.y = rand() % 26 + 2;
    DispSeed(seedColor);
}

int CheckIsEatSeed(SNAKE *snake)
{
    return (snake->head->x == seed.x && snake->head->y == seed.y);
}

char *FormatClearSnakeNodeContent(char snakeNodeContent[],SNAKENODE *snakeNode)
{
    sprintf(snakeNodeContent,"\33[%d;%dH%s\33[0m",snakeNode->y,snakeNode->x,clearSnakeNodeTag);
    return snakeNodeContent;
}

void ClearSnakeNode(SNAKENODE *delSnakeNode)
{
    char clearSnakeNodeContent[30];
    printf("%s",FormatClearSnakeNodeContent(clearSnakeNodeContent,delSnakeNode));
}

void DelSnakeNode(SNAKE *snake)
{
    SNAKENODE *delSnakeNode = snake->tail;
    if (snake->head == snake->tail)
    {
        snake->head = snake->tail = NULL;
    }else{
        snake->tail = snake->tail->next;
    }
    ClearSnakeNode(delSnakeNode);
    free(delSnakeNode);
}

void AddSnakeNode(SNAKE *snake)
{
    SNAKE *addSnake = snake;
    SNAKENODE *nextSnakeNode = (SNAKENODE *)malloc(sizeof(SNAKENODE));
    nextSnakeNode->next = NULL;
    switch (direction)
    {
    case W_UP:
        nextSnakeNode->x = addSnake->head->x;
        nextSnakeNode->y = addSnake->head->y - 1;
        break;
    case S_DOWN:
        nextSnakeNode->x = addSnake->head->x;
        nextSnakeNode->y = addSnake->head->y + 1;
        break;
    case A_LEFT:
        nextSnakeNode->x = addSnake->head->x - 2;
        nextSnakeNode->y = addSnake->head->y;
        break;
    case D_RIGHT:
        nextSnakeNode->x = addSnake->head->x + 2;
        nextSnakeNode->y = addSnake->head->y;
        break;
    }
    if (addSnake->head == NULL)
    {
        addSnake->head = addSnake->tail = nextSnakeNode;
    }else{
        addSnake->head->next = nextSnakeNode;
        addSnake->head = nextSnakeNode;
    }
}

void SnakeMove(SNAKE *snake)
{
    AddSnakeNode(snake);
    DelSnakeNode(snake);
}

char *FromatSnakeNodeContent(char snakeNodeContent[],SNAKENODE *snakeNode)
{
    sprintf(snakeNodeContent,"\33[%d;%dH%s\33[0m",snakeNode->y,snakeNode->x,snakeNodeTag);
    return snakeNodeContent;
}

void DispSnake(SNAKE *snake)
{
    char snakeNodeContent[30];
    printf("%s",FromatSnakeNodeContent(snakeNodeContent,snake->head));
}

void DispSnakeInfo(SNAKE *snake)
{
#ifdef DEBUG
    printf("\33[1;2H *DEV Model* seed.x:%3d | seed.y:%3d | snake.x:%3d | snake.y:%3d | snakeLenght:%3d",seed.x,seed.y,snake->head->x,snake->head->y,snake->snakeLength);
#else
    printf("\33[1;2H Score:%3d ",snake->snakeLength * 100 - 400);
#endif
}

void Control()
{
    char input;
    input = getch();
    switch (input)
    {
    case W_UP:
        if (direction == S_DOWN) break;
        direction = input;
        break;
    case S_DOWN:
        if (direction == W_UP) break;
        direction = input;
        break;
    case A_LEFT:
        if (direction == D_RIGHT) break;
        direction = input;
        break;
    case D_RIGHT:
        if (direction == A_LEFT) break;
        direction = input;
        break;
    case Q_QUIT:
        printf("\033[?25h");
        system("cls");
        exit(0);
        break;
    }
}

int CheckGameOver(SNAKE *snake)
{
    SNAKENODE *checkSnakeNode = snake->tail;
    do{
        if (snake->head->x == checkSnakeNode->x && snake->head->y == checkSnakeNode->y) return 1;
        if (checkSnakeNode->next->next == NULL) break;
    }while(checkSnakeNode = checkSnakeNode->next);
    return (snake->head->y == 1 || snake->head->y == windowsY || snake->head->x == 0  || snake->head->x == windowsX);
}

void DispGameOver()
{
    printf("\33[11;23H  ____    _    __  __ _____    _____     _______ ____"
           "\33[12;23H / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\"
           "\33[13;23H| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) |"
           "\33[14;23H| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <"
           "\33[15;23H \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\"
           "\33[16;%dH Hit any to exit……\33[%d;0H\n",windowsX / 2 - 9,windowsY);
    getch();
}

void InitSnake(SNAKE *snake)
{
    snake->head = snake->tail = (SNAKENODE *)malloc(sizeof(SNAKENODE));
    snake->head->x = snake->head->y = 10;
    snake->snakeLength = 1;
    int i = 3;
    while (i--)
    {
        DispSnake(snake);
        AddSnakeNode(snake);
        snake->snakeLength++;
    } 
}

void InitWindows()
{
    system("cls");
    for (int i = 1; i <= windowsY; i++)
    {
        for (int j = 0; j <= windowsX; j++)
        {
            if (i == 1 || i == windowsY) printf("%s",(i == 1 && j == 0) ? "┌" : (i == windowsY && j == windowsX) ? "┘" : (i == 1 && j == windowsX) ? "┐" : (i == windowsY && j == 0) ? "└" : "─");
            else printf("%s",(j == 0 || j == windowsX) ?  "│" : " ");
        }
        if (i != windowsY) printf("\033[?25l\n");
    }
}

void FreeSnake(SNAKE *snake)
{
    SNAKENODE *snakeNode,*freeSnakeNode = snake->tail;
    while ((snakeNode = freeSnakeNode) != NULL)
    {
        freeSnakeNode = snakeNode->next;
        free(snakeNode);
    }
}

int main()
{
    SNAKE snake = {
        .head = NULL,
        .tail = NULL,
    };
    InitWindows();
    InitSeed();
    InitSnake(&snake);
    DispSnake(&snake);
    while (1)
    {
        if (kbhit())
        {
            Control();
        }else{
            if (CheckIsEatSeed(&snake))
            {
                DispSeed(unSeedColor);
                InitSeed();
                AddSnakeNode(&snake);
                snake.snakeLength++;
            }else {
                SnakeMove(&snake);
                DispSeed(seedColor);
            }
            DispSnake(&snake);
            DispSnakeInfo(&snake);
            if (CheckGameOver(&snake)) break;
            Sleep(90);
        }
    }
    FreeSnake(&snake);
    DispGameOver();
    return 0;
}
