#include "shell.h"

/**
 * print_not_found - "<prog>: <n>: <cmd>: not found\n"
 * @prog: program name (argv[0])
 * @n: 1-based command index
 * @cmd: command string
 */
void print_not_found(const char *prog, unsigned long n, const char *cmd)
{
	char numbuf[32];
	int len;

	len = snprintf(numbuf, sizeof(numbuf), "%lu", n);
	(void)len;

	write(STDERR_FILENO, prog, strlen(prog));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, numbuf, strlen(numbuf));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, cmd, strlen(cmd));
	write(STDERR_FILENO, ": not found\n", 12);
}
