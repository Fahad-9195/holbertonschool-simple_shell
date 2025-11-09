#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

/* Prompt used in interactive mode */
#define PROMPT "#cisfun$ "

/**
 * run_shell - Main REPL loop (interactive & non-interactive)
 * @progname: program name (argv[0]) used in error messages
 *
 * Return: last child exit status or 0
 */
int run_shell(char *progname);

#endif /* SHELL_H */
