/*
Assignment name		:	sandbox
Expected files		:	sandbox.c
Allowed functions	:	fork, waitpid, exit, alarm, sigaction, kill,
						printf, strsignal, errno
===============================================================================

Write the following function:

#include <stdbool.h>
int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)

This function must test if the function f is a nice function or a bad function,
you will return 1 if f is nice , 0 if f is bad or -1 in case of an error in
your function.

A function is considered bad if it is terminated or stopped by a signal
(segfault, abort...), if it exit with any other exit code than 0 or if it
times out.

If verbose is true, you must write the appropriate message among the following:

"Nice function!\n"
"Bad function: exited with code <exit_code>\n"
"Bad function: <signal description>\n"
"Bad function: timed out after <timeout> seconds\n"

You must not leak processes (even in zombie state, this will be checked using
wait).

We will test your code with very bad functions.
*/

// sandbox.c

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> 

void	do_nothing(int sig)
{
	(void)sig;
}
// *** how does it work: ***
// Of course, you fork a child, and inside the child, the function is tested.
// Then in the parent, set the signal handler to detect signal from alarm() -- SIGALARM
// When you set the sa_handler = do_nothing, the SIGALARM from alarm interrupts waitpid.
// The waitpid stops with an error (EINTR) that can be detected afterwards, and the child is sent a SIGKILL.
// There is no need to set the alarm in the child process, because the parent handles is anyways.

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		f();
		exit(0);
	}

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = do_nothing;
	if (sigaction(SIGALRM, &sa, NULL) < 0)
		return (-1);
	alarm(timeout);

	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %d seconds\n", timeout);
			return (0);
		}
		return (-1);
	}
	alarm(0);
	if (WIFEXITED(status))
	{
		int code = WEXITSTATUS(status);
		if (code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else if (verbose)
			printf("Bad function: exited with code %d\n", code);
		return (0);
	}
	if (WIFSIGNALED(status))
	{
		if (verbose)
			printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
		return (0);
	}
	return (-1);
}

void slow_function(void) {
    alarm(2); // Set timeout for 2 seconds
    for (volatile long i = 0; i < 1e9; ++i); // Busy work
    printf("This will probably not print\n");
}


void good_func()
{
	printf("I'm good!\n");
}

int main(void) {
	sandbox(good_func, 1, true);
    // slow_function();
    return 0;
}