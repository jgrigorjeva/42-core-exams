#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This file is to help you test your picoshell. Something similar will be provided at the exam.
// Declare the picoshell function
int		picoshell(char **cmds[]);

// Helper to count commands (split by "|")
int	count_commands(int argc, char **argv)
{
	int	count;

	count = 1;
	for (int i = 1; i < argc; ++i)
		if (strcmp(argv[i], "|") == 0)
			count++;
	return (count);
}

// Helper to split the arguments into command arrays
char	***build_cmds(int argc, char **argv)
{
	int	cmd_count;
	int	cmd_index;
	int	arg_end;
	int	arg_count;

	cmd_count = count_commands(argc, argv);
	char ***cmds = calloc(cmd_count + 1, sizeof(char **)); // NULL-terminated
	if (!cmds)
	{
		perror("calloc cmds");
		exit(1);
	}
	cmd_index = 0;
	int arg_start = 1; // skip argv[0]
	while (arg_start < argc)
	{
		// Find next "|"
		arg_end = arg_start;
		while (arg_end < argc && strcmp(argv[arg_end], "|") != 0)
			arg_end++;
		// Count args in this command
		arg_count = arg_end - arg_start;
		char **cmd = calloc(arg_count + 1, sizeof(char *)); // NULL-terminated
		if (!cmd)
		{
			perror("calloc cmd");
			exit(1);
		}
		for (int i = 0; i < arg_count; ++i)
			cmd[i] = argv[arg_start + i];
		cmds[cmd_index++] = cmd;
		arg_start = arg_end + 1; // skip "|"
	}
	cmds[cmd_index] = NULL;
	return (cmds);
}

// Helper to free cmds
void	free_cmds(char ***cmds)
{
	if (!cmds)
		return ;
	for (int i = 0; cmds[i]; ++i)
		free(cmds[i]); // only free the array, not the strings (they're argv)
	free(cmds);
}

int	main(int argc, char **argv)
{
	char	***cmds;
	int		ret;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s cmd1 [args...] '|' cmd2 [args...] ...\n",
			argv[0]);
		return (1);
	}
	cmds = build_cmds(argc, argv);
	ret = picoshell(cmds);
	free_cmds(cmds);
	return (ret);
}
