#include "shell.h"

/**
 * _print_prompt - print prompt if running interactively
 */
static void _print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);
}

/**
 * _trim_newline - replace trailing newline with '\0'
 * @s: string line from getline
 */
static void _trim_newline(char *s)
{
	size_t len;

	if (!s)
		return;
	len = strlen(s);
	if (len && s[len - 1] == '\n')
		s[len - 1] = '\0';
}

/**
 * _execute_oneword - fork/exec a single-word command without PATH
 * @cmd: command path (absolute or relative) or a bare word
 * @progname: argv[0] for perror prefix
 *
 * Return: child's exit status (0..255), or 127 if exec failed before fork
 */
static int _execute_oneword(char *cmd, char *progname)
{
	pid_t pid;
	int status = 0;
	char *argvv[2];

	if (!cmd || *cmd == '\0')
		return (0);

	argvv[0] = cmd;
	argvv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		return (1);
	}
	if (pid == 0)
	{
		/* No PATH resolution: try execve directly on given word */
		if (execve(cmd, argvv, environ) == -1)
		{
			/* match sample: "./shell: No such file or directory" */
			perror(progname);
			_exit(127);
		}
		_exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		perror(progname);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

/**
 * run_shell - Main loop: prompt, read line, run single-word command
 * @progname: argv[0]
 *
 * Return: last exit status
 */
int run_shell(char *progname)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t r;
	int status = 0;

	while (1)
	{
		_print_prompt();

		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			/* Handle EOF (Ctrl+D) gracefully */
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		_trim_newline(line);

		/* For 0.1: command lines are exactly one word */
		if (line[0] == '\0')
			continue;

		status = _execute_oneword(line, progname);
	}
	free(line);
	return (status);
}

/**
 * main - Entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: process exit status
 */
int main(int argc, char **argv)
{
	(void)argc;
	return (run_shell(argv[0]));
}
