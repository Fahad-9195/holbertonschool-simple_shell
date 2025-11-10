#include "shell.h"

/**
 * builtin_env - print current environment (one VAR=VALUE per line)
 */
void builtin_env(void)
{
	size_t i;

	for (i = 0; environ && environ[i]; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
}

/**
 * handle_builtins - check and run builtins (exit, env)
 * @argv: argv parsed for the command line
 * @should_exit: set to 1 when "exit" is requested
 * @status_out: set to desired status for the builtin (usually 0)
 *
 * Return: 1 if a builtin was handled, 0 otherwise
 */
int handle_builtins(char **argv, int *should_exit, int *status_out)
{
	if (!argv || !argv[0])
		return (0);

	if (strcmp(argv[0], "exit") == 0)
	{
		if (should_exit)
			*should_exit = 1;
		if (status_out)
			*status_out = 0; /* 0.4: لا نحتاج دعم exit مع أرقام */
		return (1);
	}

	if (strcmp(argv[0], "env") == 0)
	{
		builtin_env();
		if (status_out)
			*status_out = 0;
		return (1);
	}

	return (0);
}
