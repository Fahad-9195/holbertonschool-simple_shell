#include "shell.h"

/**
 * _print_prompt - print prompt if interactive
 */
static void _print_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);
}

/**
 * spawn_exec - fork and exec using resolved full path
 */
static int spawn_exec(char **argvv, char *full, const char *prog)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid == -1)
	{
		perror(prog);
		return (1);
	}
	if (pid == 0)
	{
		argvv[0] = full;
		if (execve(argvv[0], argvv, environ) == -1)
		{
			perror(prog);
			_exit(127);
		}
		_exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		perror(prog);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

/**
 * exec_line - tokenize, resolve PATH, exec or handle built-ins
 */
static int exec_line(char *line, const char *prog, unsigned long n)
{
	char *argvv[ARGV_MAX], *full;
	int argc, status;

	argc = build_argv(line, argvv, ARGV_MAX);
	if (argc == 0)
		return (0);

	/* BUILT-IN: exit */
	if (strcmp(argvv[0], "exit") == 0)
		exit(0);

	full = resolve_cmd(argvv[0]);
	if (!full)
	{
		print_not_found(prog, n, argvv[0]);
		return (127);
	}

	status = spawn_exec(argvv, full, prog);
	free(full);
	return (status);
}

/**
 * run_shell - main loop
 */
int run_shell(char *progname)
{
	char *line = NULL, *cmdline;
	size_t n = 0;
	ssize_t r;
	int status = 0;
	unsigned long cmd_no = 0;

	while (1)
	{
		_print_prompt();
		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}
		cmdline = sanitize_line(line);
		if (!cmdline || cmdline[0] == '\0')
			continue;
		cmd_no++;
		status = exec_line(cmdline, progname, cmd_no);
	}
	free(line);
	return (status);
}

/**
 * main - Entry
 */
int main(int argc, char **argv)
{
	(void)argc;
	return (run_shell(argv[0]));
}
