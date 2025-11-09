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

#define PROMPT "#cisfun$ "
#define ARGV_MAX 256

/* tokenizer.c */
char *sanitize_line(char *s);
int build_argv(char *line, char **argv, size_t max);

/* path.c */
char *resolve_cmd(const char *cmd);

/* errors.c */
void print_not_found(const char *prog, unsigned long n, const char *cmd);

/* main.c */
int run_shell(char *progname);

#endif /* SHELL_H */
