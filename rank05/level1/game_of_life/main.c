#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	print_board(char **board, int width, int height);
void	free_board(char **board, int height);
void play_game(char **board, int width, int height, int iterations);
int		get_neighbours(char **board, int width, int height, int i, int j);
int		get_value(char **board, int width, int height, int i, int j);

int	main(int argc, char **argv)
{
	int		width;
	int		height;
	int		iterations;
	char	buf;
	int		draw, x, y;
	char	**board;
	if (argc != 4)
		return (1);
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	if (!width || !height)
		return (1);
	iterations = atoi(argv[3]);
	board = calloc(height, sizeof(char *));
	if (!board)
		return (2);
	for (int i = 0; i < height; i++)
	{
		board[i] = calloc(width, sizeof(char));
		if (!board[i])
		{
			free_board(board, height);
			return (2);
		}
		for (int j = 0; j < width; j++)
			board[i][j] = ' ';
	}
	draw = 0, x = 0, y = 0;
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
			if (draw == 0)
				draw = 1;
			else if (draw == 1)
				draw = 0;
		}
		if (draw)
			board[y][x] = 'O';
	}
	play_game(board, width, height, iterations);
	free_board(board, height);
}

void	print_board(char **board, int width, int height)
{
	if (!board)
	{
		write(STDERR_FILENO, "board is NULL\n", 14);
		return ;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			write(STDOUT_FILENO, &(board[i][j]), 1);
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	free_board(char **board, int height)
{
	for (int i = 0; i < height; i++)
	{
		if (!board[i])
			break ;
		free(board[i]);
	}
	free(board);
}

void play_game(char **board, int width, int height, int iterations)
{
	int	neighbours;
	char **temp_board = calloc(height, sizeof(char *));
	if (!temp_board)
		return ;
	for (int i = 0; i < height; i++)
	{
		temp_board[i] = calloc(width, sizeof(char));
		if (!temp_board[i])
		{
			free_board(temp_board, height);
			return ;
		}
	}
	for (int it = 0; it < iterations; it++)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				temp_board[i][j] = ' ';
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				neighbours = get_neighbours(board, width, height, i, j);
				if (board[i][j] == 'O')
				{
					if (neighbours < 2)
						temp_board[i][j] = ' ';
					else if (neighbours <= 3)
						temp_board[i][j] = 'O';
					else if (neighbours > 3)
						temp_board[i][j] = ' ';
				}
				if (board[i][j] == ' ' && neighbours == 3)
					temp_board[i][j] = 'O';
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				board[i][j] = temp_board[i][j];
		}
	}
	print_board(board, width, height);
}

int	get_neighbours(char **board, int width, int height, int i, int j)
{
	int	neighbours;

	neighbours = 0;
	neighbours += get_value(board, width, height, i + 1, j);
	neighbours += get_value(board, width, height, i, j + 1);
	neighbours += get_value(board, width, height, i + 1, j + 1);
	neighbours += get_value(board, width, height, i - 1, j);
	neighbours += get_value(board, width, height, i, j - 1);
	neighbours += get_value(board, width, height, i - 1, j - 1);
	neighbours += get_value(board, width, height, i - 1, j + 1);
	neighbours += get_value(board, width, height, i + 1, j - 1);
	return (neighbours);
}
int	get_value(char **board, int width, int height, int i, int j)
{
	if (i < 0 || j < 0 || i >= height || j >= width)
		return (0);
	if (board[i][j] == 'O')
		return (1);
	return (0);
}