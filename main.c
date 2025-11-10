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
 * spawn_exec - fork and exec using a resolved full path
 * @argvv: argument vector (argv[0] will be replaced by full)
 * @full: resolved absolute path to executable
 * @prog: program name for perror
 * Return: child's exit status (0..255) or 1 on fork error
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
 * exec_line - tokenize, run builtins, resolve PATH, then exec
 * @line: sanitized line (modified in-place)
 * @prog: program name for errors
 * @n: 1-based command index for error printing
 * @want_exit: set to 1 if "exit" builtin requested
 * Return: status code (0..255), or 127 if not found
 */
static int exec_line(char *line, const char *prog,
		     unsigned long n, int *want_exit)
{
	char *argvv[ARGV_MAX], *full;
	int argc, handled, status_builtin = 0;

	argc = build_argv(line, argvv, ARGV_MAX);
	if (argc == 0)
		return (0);

	handled = handle_builtins(argvv, want_exit, &status_builtin);
	if (handled)
		return (status_builtin);

	full = resolve_cmd(argvv[0]);
	if (!full)
	{
		print_not_found(prog, n, argvv[0]);
		return (127);
	}

	/* normal exec path */
	{
		int st = spawn_exec(argvv, full, prog);

		free(full);
		return (st);
	}
}

/**
 * run_shell - prompt/read/exec loop
 * @progname: argv[0]
 * Return: last status
 */
int run_shell(char *progname)
{
	char *line = NULL, *cmdline;
	size_t n = 0;
	ssize_t r;
	int status = 0, should_exit = 0;
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
		status = exec_line(cmdline, progname, cmd_no, &should_exit);
		if (should_exit)
			break;
	}
	free(line);
	return (status);
}

/**
 * main - Entry
 * @argc: arg count
 * @argv: arg vector
 * Return: exit status
 */
int main(int argc, char **argv)
{
	(void)argc;
	return (run_shell(argv[0]));
}
