#include "shell.h"
#include <unistd.h>

/**
 * run_shell - Temporary stub that prints nothing and exits
 * @progname: program name (unused for now)
 *
 * Return: 0
 */
int run_shell(char *progname)
{
	(void)progname;
	return (0);
}

/**
 * main - Entry point for the simple shell program
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: process exit status (0 on success)
 */
int main(int argc, char **argv)
{
	(void)argc;
	return (run_shell(argv[0]));
}
