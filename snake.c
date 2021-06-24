#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

typedef struct snakeNode{
    int x;
    int y;
    struct snakeNode *next;
}SNAKE;

typedef struct {
    int x;
    int y;
}FEED;

enum key {
    W_UP    = 119,
    A_LEFT  = 97,
    S_DOWN  = 115,
    D_RIGHT = 100,
    Q_QUIT  = 81,
};

int windowsX = 100,windowsY = 30;
char snakeNodeTag[5] = "⬜";
char clearSnakeNodeTag[5] = "  ";
char direction = 'w';
FEED feed;


void InitFeed()
{
    srand((unsigned)time(NULL));
    feed.x = (rand() % 96)/2 + 2;
    feed.x += (feed.x % 2);
    feed.y = rand() % 28 + 2;
}

char *FeedContent(char feedContent[])
{
    sprintf(feedContent,"\33[%d;%dH%s\33[0m",feed.y,feed.x,snakeNodeTag);
    return feedContent;
}

void DispFeed()
{
    char feedContent[30];
    printf("%s",FeedContent(feedContent));   
}

int CheckEatFeed(SNAKE *snakeHead)
{
    while ((snakeHead = snakeHead->next) != 0)
    return (snakeHead->x == feed.x && snakeHead->y == feed.y) ? 1 : 0;
}

char *FormatClearSnakeNodeContent(char snakeNodeContent[],SNAKE *snakeNode)
{
    sprintf(snakeNodeContent,"\33[%d;%dH%s\33[0m",snakeNode->y,snakeNode->x,clearSnakeNodeTag);
    return snakeNodeContent;
}

void ClearSnakeNode(SNAKE *delSnakeNode)
{
    char clearSnakeNodeContent[30];
    printf("%s",FormatClearSnakeNodeContent(clearSnakeNodeContent,delSnakeNode));
}

SNAKE *DelSnakeNode(SNAKE *snakeHead)
{
    SNAKE *delSnakeNode = snakeHead;
    snakeHead = delSnakeNode->next;
    ClearSnakeNode(delSnakeNode);
    free(delSnakeNode);
    return snakeHead;
}

SNAKE *AddSnakeNode(SNAKE *snakeHead)
{
    SNAKE *AddSnakeNode = snakeHead;
    while (AddSnakeNode->next != NULL)
    {
        AddSnakeNode = AddSnakeNode->next;
    }
    AddSnakeNode->next = (SNAKE *)malloc(sizeof(SNAKE));
    AddSnakeNode->next->next = NULL;
    switch (direction)
    {
    case W_UP:
        AddSnakeNode->next->x = AddSnakeNode->x;
        AddSnakeNode->next->y = AddSnakeNode->y - (AddSnakeNode->y - 1 > 1);
        break;
    case S_DOWN:
        AddSnakeNode->next->x = AddSnakeNode->x;
        AddSnakeNode->next->y = AddSnakeNode->y + (AddSnakeNode->y + 1 < windowsY);
        break;
    case A_LEFT:
        AddSnakeNode->next->x = AddSnakeNode->x - (2 * (AddSnakeNode->x - 2 > 0));
        AddSnakeNode->next->y = AddSnakeNode->y;
        break;
    case D_RIGHT:
        AddSnakeNode->next->x = AddSnakeNode->x + (2 * (AddSnakeNode->x + 2 < windowsX));
        AddSnakeNode->next->y = AddSnakeNode->y;
        break;
    }
    return snakeHead;
}

SNAKE *SnakeMove(SNAKE *snake)
{
    snake = AddSnakeNode(snake);
    snake = DelSnakeNode(snake);
    return snake;
}

char *FromatSnakeNodeContent(char snakeNodeContent[],SNAKE *snakeNode)
{
    sprintf(snakeNodeContent,"\33[%d;%dH%s\33[0m",snakeNode->y,snakeNode->x,snakeNodeTag);
    return snakeNodeContent;
}

void DispSnake(SNAKE *snakeHead)
{
    char snakeNodeContent[30];
    while ((snakeHead = snakeHead->next) != NULL)
    printf("%s",FromatSnakeNodeContent(snakeNodeContent,snakeHead)); 
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

SNAKE *InitSnake()
{
    SNAKE *snake = (SNAKE *)malloc(sizeof(SNAKE));
    snake->x = snake->y = 10;
    snake->next = NULL;
    for (int i = 0; i < 3; i++)
    {
       snake = AddSnakeNode(snake);
    }
    return snake;
}

void InitWindows()
{
    for (int i = 0; i <= windowsY; i++)
    {
        for (int j = 0; j <= windowsX; j++)
        {
            if (i == 1 || i == windowsY) printf("%s",(i == 1 && j == 0) ? "┌" : (i == windowsY && j == windowsX) ? "┘" : (i == 1 && j == windowsX) ? "┐" : (i == windowsY && j == 0) ? "└" : "─");
            else printf("%s",(j == 0 || j == windowsX) ?  "│" : " ");
        }
        if (i != windowsY) printf("\033[?25l\n");
    }
    
}

int main()
{
    SNAKE *snake = InitSnake();
    InitFeed();
    InitWindows();
    while (1)
    {
        if (kbhit())
        {
            Control();
        }else{
            printf("\33[1;2H *DEV Model* feed.x:%3d | feed.y:%3d | snake.x:%3d | snake.y:%3d",feed.x,feed.y,snake->next->x,snake->next->y);
            snake = SnakeMove(snake);
            if (CheckEatFeed(snake))
            {
                snake = AddSnakeNode(snake);
                InitFeed();
            }
            DispFeed();
            DispSnake(snake);
            Sleep(60);
        }
    }
}
