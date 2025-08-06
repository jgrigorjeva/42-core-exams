#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	arg_not_matching(const char *file, char *const argv[]);

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !file[0] || !argv || !argv[0] || !(type == 'r' || type == 'w'))
		return (-1);
	if (arg_not_matching(file, argv))
		return (-1);
	if (pipe(fd))
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
		
	if (pid == 0)
	{
		if (type == 'r')
		{
			close(fd[0]);
			if (dup2(fd[1], STDOUT_FILENO) == -1)
				exit (1);
			close(fd[1]);
		}
		else
		{
			close(fd[1]);
			if (dup2(fd[0], STDIN_FILENO) == -1)
				exit (1);
			close(fd[0]);
		}
		execvp(file, argv);
		exit (1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else {
		close(fd[0]);
		return (fd[1]);
	}
	return (0);
}

int	main(void)
{
	int		fd;
	char	buf[1];

	fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
	while (read(fd, buf, 1))
	{
		write(1, buf, 1);
		// read(fd, &buf, 1);
	}
}

int	arg_not_matching(const char *file, char *const argv[])
{
	int	i;

	i = 0;
	while (file[i] || argv[0][i])
	{
		if (file[i] != argv[0][i])
		{
			// write(1, "they do not match\n", 18);
			return (1);
		}
		i++;
	}
	// write(1, "they match\n", 11);
	return (0);
}
