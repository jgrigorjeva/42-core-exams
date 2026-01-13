#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct s_bsq
{
	char **map;
	int expectedlines;
	int reallines;
	char empty;
	char obstacle;
	char full;
	FILE *mapfile;
	int x0;
	int y0;
	int plusx;
	int plusy;
	size_t linelen;

} t_bsq;

void bsq(char *map_filename);
t_bsq init_bsq();
void print_error();
void print_map(t_bsq bsq);

int	main(int argc, char **argv)
{
	if (argc == 1)
		bsq(NULL);
	else
	{
		for (int i = 1; i < argc; i++)
			bsq(argv[i]);
	}
	return (0);
}

// ssize_t getline(char **lineptr, size_t *n, FILE *stream);
// the stdin has a type FILE* -> can be used directly in getline
void bsq(char *map_filename)
{
	t_bsq bsq = init_bsq();

	if (map_filename == NULL)
		bsq.mapfile = stdin;
	else
		bsq.mapfile = fopen(map_filename, "r");
	fscanf(bsq.mapfile, "%d %c %c %c", &(bsq.expectedlines), &(bsq.empty), &(bsq.obstacle), &(bsq.full));
	printf("First line: %d, %c, %c, %c\n", bsq.expectedlines, bsq.empty, bsq.obstacle, bsq.full);

	char *line = NULL;
	size_t len = 0;
	while (getline(&line, &len, bsq.mapfile) != -1)
	{
		printf("getline, line number: %i\n", bsq.reallines);
		bsq.map = realloc(bsq.map, (bsq.reallines + 1) * sizeof(char *));
		printf("getline, map realloc\n");
		// do safety check
		bsq.map[bsq.reallines++] = line;
		printf("getline, line assigned\n");

		
		if (len > 0 && line[len-1] == '\n')
		{
			if (bsq.linelen == 0)
				bsq.linelen = len - 1;
			else if (bsq.linelen != len - 1)
			{
				print_error();
				return ;
			}			
		}
		len = 0;
		line = NULL;
	}
	print_map(bsq);
}

t_bsq init_bsq()
{
	t_bsq bsq;
	bsq.empty = '.';
	bsq.full = 'x';
	bsq.obstacle = 'o';
	bsq.map = NULL;
	bsq.mapfile = NULL;
	bsq.x0 = 0;
	bsq.y0 = 0;
	bsq.plusx = 0;
	bsq.plusy = 0;
	bsq.expectedlines = 0;
	bsq.reallines = 0;
	bsq.linelen = 0;
	return bsq;
}

void print_map(t_bsq bsq)
{
	printf("print map/n");
	if (!bsq.map)
		return ;
	for (int i = 0; i < bsq.expectedlines && bsq.map[i]; i++)
	{
		for (unsigned int j = 0; j <bsq.linelen; j++)
			write(STDOUT_FILENO, &(bsq.map[j][i]), 1);
		write(STDOUT_FILENO, "/n", 1);
	}

}

void print_error()
{
	write(STDOUT_FILENO, "Map error\n", 10);
}