#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void bsq(char *map_filename);

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
	// char **map = NULL;
	int lines;
	char empty;
	char obstacle;
	char full;
	// char **lineptr = NULL;
	// size_t n = 0;
	// ssize_t chars_read;
	FILE* mapfile = NULL;
	// char mode = 'r';

	if (map_filename == NULL)
		mapfile = stdin;
	else
		mapfile = fopen(map_filename, "r");
	fscanf(mapfile, "%d %c %c %c", &lines, &empty, &obstacle, &full);
	// chars_read = getline(lineptr, &n, mapfile);
	// if (chars_read < 7)
	// {
	// 	write(STDOUT_FILENO, "map error\n", 10);
	// 	return ;
	// }
	printf("First line: %d, %c, %c, %c\n", lines, empty, obstacle, full);
	// (void)chars_read;
	
}