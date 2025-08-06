#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Allowed functions:    close, fork, wait, exit, execvp, dup2, pipe
// You need to check that the fd is open before closing it!
int	picoshell(char **cmds[])
{
	int		i;
	pid_t	pid;
	int		prev_pipe[2];
	int		next_pipe[2];
	int		status;
	int		ret;

	i = 0;
	if (!cmds || !cmds[0])
		return (1);
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(next_pipe) == -1)
			{
				if (i > 0)
				{
					close(prev_pipe[0]);
					close(prev_pipe[1]);
				}
				return (1);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			if (i > 0)
			{
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (cmds[i + 1])
			{
				close(next_pipe[0]);
				close(next_pipe[1]);
			}
		}
		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(prev_pipe[0], STDIN_FILENO);
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (cmds[i + 1])
			{
				dup2(next_pipe[1], STDOUT_FILENO);
				close(next_pipe[0]);
				close(next_pipe[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (i > 0)
		{
			close(prev_pipe[0]);
			close(prev_pipe[1]);
		}
		if (cmds[i + 1])
		{
			prev_pipe[0] = next_pipe[0];
			prev_pipe[1] = next_pipe[1];
		}
		i++;
	}
	i = 0;
	ret = 0;
	while (cmds[i++])
	{
		wait(&status);
		if ((WIFEXITED(status) && WEXITSTATUS(status)) || !WIFEXITED(status)) // I am not sure I checked for this at the exam, so it might not be necessary.
			ret = 1;
	}
	return (ret);
}
