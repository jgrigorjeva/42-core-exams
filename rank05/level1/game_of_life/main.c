#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void print_board(char **board, int width, int height);

int main(int argc, char **argv)
{
    if (argc != 4)
        return 1;
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    if (!width || !height)
        return 1;
    int iterations = atoi(argv[3]);
    
    char board[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            board[i][j] = ' ';
    }
    char buf;
    int draw = 0, x = 0, y = 0;
    while (read(STDIN_FILENO, &buf, 1))
    {
        if (buf == 'w' && y > 0)
            y--;
        else if (buf == 'a' && x > 0)
            x--;
        else if (buf == 's' && y < height)
            y++;
        else if (buf == 'd' && x < width)
            x++;
        else if (buf == 'x')
        {

            // printf("x detected, draw was: %i", draw);

            if (draw == 0)
                draw = 1;
            else if (draw == 1)
                draw = 0;
            // printf("draw now is: %i", draw);
        }
        if (draw)
            board[y][x] = 'O';
    }
    // print_board(board, width, height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            write(STDOUT_FILENO, &(board[i][j]), 1);
        write(STDOUT_FILENO, "\n", 1);

    }
    (void)iterations;
}

void print_board(char **board, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            write(STDOUT_FILENO, &(board[j][i]), 1);
        write(STDOUT_FILENO, "\n", 1);
    }
}