#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

#define W 40
#define H 20

typedef struct
{
    int x;
    int y;
} Node;

typedef struct
{
    int length;
    Node direction;
    Node body[W * H];
} Snake;

char key;

void *key_event_loop(void *vargp)
{
    while (1)
    {
        key = getch();
        // usleep((1 / 15.0) * 1000000);
    }
}

void clearBuffer(char buffer[H][W])
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            buffer[i][j] = ' ';
        }
    }
}

void marge(char buffer[H][W], Snake snake, Node food)
{
    buffer[food.y][food.x] = '*';

    for (int i = 0; i < snake.length; i++)
    {
        int x = snake.body[i].x;
        int y = snake.body[i].y;

        buffer[y][x] = '$';
    }
}

void show(char buffer[H][W])
{
    for (int k = 0; k < (W + 2); k++)
    {
        printf("#");
    }
    printf("\r\n");
    for (int i = 0; i < H; i++)
    {
        printf("#");
        for (int j = 0; j < W; j++)
        {
            printf("%c", buffer[i][j]);
        }
        printf("#\r\n");
    }
    for (int k = 0; k < (W + 2); k++)
    {
        printf("#");
    }
    printf("\r\n");
}

void moveSnake(Snake *snake)
{
    printf("teste");
    for (int i = snake->length - 1; i > 0; i--)
    {
        snake->body[i].x = snake->body[i - 1].x;
        snake->body[i].y = snake->body[i - 1].y;
    }

    int x = (snake->body[0].x + snake->direction.x) % W;
    int y = (snake->body[0].y + snake->direction.y) % H;

    snake->body[0].x = x >= 0 ? x : W - x - 1;
    snake->body[0].y = y >= 0 ? y : H - y - 1;
}

void eat(Snake *snake)
{
    if (snake->length > W * H)
        return;

    Node new = snake->body[snake->length - 1];
    moveSnake(snake);
    snake->body[snake->length] = new;
    snake->length++;
}

void check_eat(Snake *snake, Node *food)
{
    if (
        snake->body[0].x == food->x &&
        snake->body[0].y == food->y)
    {
        eat(snake);
        int x = rand() % W;
        int y = rand() % H;
        food->x = x;
        food->y = y;
    }
}

int main()
{
    srand(time(NULL));

    initscr();
    cbreak();
    noecho();

    pthread_t thread_id;

    char buffer[H][W];
    Snake snake;

    Node food;
    food.x = rand() % W;
    food.y = rand() % H;

    snake.length = 1;

    snake.direction.x = 1;
    snake.direction.y = 0;

    snake.body[0].x = 5;
    snake.body[0].y = 5;

    pthread_create(&thread_id, NULL, key_event_loop, NULL);

    while (1)
    {
        clear();
        refresh();
        clearBuffer(buffer);

        check_eat(&snake, &food);

        marge(buffer, snake, food);
        printf("Direction: \r\n");
        printf("\tx: %d\ty: %d\r\n", snake.direction.x, snake.direction.y);
        printf("Position: \r\n");
        printf("\tx: %d\ty: %d\r\n", snake.body[0].x, snake.body[0].y);
        printf("Length: \r\n");
        printf("\t%d\r\n", snake.length);
        printf("Food Position: \r\n");
        printf("\tx: %d\ty: %d\r\n", food.x, food.y);
        show(buffer);

        if (key != EOF)
        {
            switch (key)
            {
            case 'w':
            {
                snake.direction.x = 0;
                snake.direction.y = -1;
                break;
            }
            case 's':
            {
                snake.direction.x = 0;
                snake.direction.y = 1;
                break;
            }
            case 'a':
            {
                snake.direction.x = -1;
                snake.direction.y = 0;
                break;
            }
            case 'd':
            {
                snake.direction.x = 1;
                snake.direction.y = 0;
                break;
            }
            default:
                break;
            }
        }

        moveSnake(&snake);

        usleep((1 / (8.0 + snake.length)) * 1000000);
    }

    endwin();
    return 0;
}