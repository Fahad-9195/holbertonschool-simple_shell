#include "shell.h"

/**
 * _sanitize_line - trim trailing newline and outer spaces/tabs
 * @s: line from getline (modified in-place)
 *
 * Return: pointer to first non-space char inside s (may be s itself)
 */
static char *_sanitize_line(char *s)
{
	size_t i = 0, end;

	if (!s)
		return (NULL);

	end = strlen(s);
	if (end && s[end - 1] == '\n')
		s[--end] = '\0';

	while (s[i] == ' ' || s[i] == '\t')
		i++;

	end = strlen(s + i);
	while (end && (s[i + end - 1] == ' ' || s[i + end - 1] == '\t'))
		end--;
	s[i + end] = '\0';

	return (s + i);
}

/**
 * _resolve_cmd - resolve argv[0] using PATH (or direct path if contains '/')
 * @cmd: command name from argv[0]
 *
 * Return: malloc'ed full path to executable, or NULL if not found
 *
 * Note: Caller must free the returned pointer if not NULL.
 */
static char *_resolve_cmd(const char *cmd)
{
	char *path_env = NULL, *copy = NULL, *dir = NULL, *full = NULL;
	size_t i, dlen, clen;

	if (!cmd || *cmd == '\0')
		return (NULL);

	/* If command has a slash, try it directly (no PATH search) */
	for (i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '/')
		{
			clen = strlen(cmd);
			full = malloc(clen + 1);
			if (!full)
				return (NULL);
			memcpy(full, cmd, clen + 1);
			if (access(full, X_OK) == 0)
				return (full);
			free(full);
			errno = ENOENT;
			return (NULL);
		}
	}

	/* Locate PATH in environ */
	for (i = 0; environ && environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path_env = environ[i] + 5;
			break;
		}
	}
	if (!path_env || *path_env == '\0')
	{
		errno = ENOENT;
		return (NULL);
	}

	/* Work on a copy of PATH because strtok modifies the string */
	copy = malloc(strlen(path_env) + 1);
	if (!copy)
		return (NULL);
	memcpy(copy, path_env, strlen(path_env) + 1);

	clen = strlen(cmd);
	dir = strtok(copy, ":");
	while (dir)
	{
		dlen = strlen(dir);
		/* build dir + "/" + cmd + NUL */
		full = malloc(dlen + 1 + clen + 1);
		if (!full)
		{
			free(copy);
			return (NULL);
		}
		memcpy(full, dir, dlen);
		full[dlen] = '/';
		memcpy(full + dlen + 1, cmd, clen + 1);

		if (access(full, X_OK) == 0)
		{
			free(copy);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":");
	}
	free(copy);
	errno = ENOENT;
	return (NULL);
}

/**
 * _execute_line - fork/exec command line, with PATH resolution
 * @line: sanitized command line (tokenized in-place)
 * @progname: argv[0] for error prefix
 * @cmd_no: 1-based command index for error printing
 *
 * Return: child's exit status (0..255), or 127 if command not found
 */
static int _execute_line(char *line, char *progname, unsigned long cmd_no)
{
	char *argvv[256], *tok, *full = NULL;
	size_t i = 0;
	pid_t pid;
	int status = 0;

	if (!line || *line == '\0')
		return (0);

	/* tokenize by spaces/tabs only (no quotes/escapes in 0.3) */
	tok = strtok(line, " \t");
	while (tok && i + 1 < (sizeof(argvv) / sizeof(argvv[0])))
	{
		argvv[i++] = tok;
		tok = strtok(NULL, " \t");
	}
	argvv[i] = NULL;

	if (i == 0)
		return (0);

	/* Resolve before forking; if not found, print "<prog>: <n>: <cmd>: not found" */
	full = _resolve_cmd(argvv[0]);
	if (!full)
	{
		char numbuf[32];
		(void)sprintf(numbuf, "%lu", cmd_no);
		write(STDERR_FILENO, progname, strlen(progname));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, numbuf, strlen(numbuf));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, argvv[0], strlen(argvv[0]));
		write(STDERR_FILENO, ": not found\n", 12);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror(progname);
		free(full);
		return (1);
	}
	if (pid == 0)
	{
		argvv[0] = full; /* use resolved path */
		if (execve(argvv[0], argvv, environ) == -1)
		{
			perror(progname);
			_exit(127);
		}
		_exit(0);
	}
	if (waitpid(pid, &status, 0) == -1)
		perror(progname);

	free(full);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
}

/**
 * run_shell - Main loop: prompt, read, sanitize, resolve PATH, exec
 * @progname: argv[0]
 *
 * Return: last exit status
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
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

		r = getline(&line, &n, stdin);
		if (r == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		cmdline = _sanitize_line(line);
		if (!cmdline || cmdline[0] == '\0')
			continue;

		cmd_no++;
		status = _execute_line(cmdline, progname, cmd_no);
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
