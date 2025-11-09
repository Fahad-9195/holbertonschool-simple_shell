#include "shell.h"

/**
 * try_direct - try executing command as a direct path (has '/')
 * @cmd: command string
 * Return: malloc'ed full path if executable, else NULL (sets errno to ENOENT)
 */
static char *try_direct(const char *cmd)
{
	size_t clen;
	char *full;

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

/**
 * get_path_value - get PATH= value from environ
 * Return: pointer inside environ to PATH value or NULL
 */
static const char *get_path_value(void)
{
	size_t i;

	for (i = 0; environ && environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
	}
	return (NULL);
}

/**
 * build_candidate - join dir and cmd into "dir/cmd"
 * @dir: directory string (non-NULL)
 * @cmd: command string (non-NULL)
 * Return: malloc'ed string or NULL
 */
static char *build_candidate(const char *dir, const char *cmd)
{
	size_t dlen, clen;
	char *full;

	dlen = strlen(dir);
	clen = strlen(cmd);

	full = malloc(dlen + 1 + clen + 1);
	if (!full)
		return (NULL);

	memcpy(full, dir, dlen);
	full[dlen] = '/';
	memcpy(full + dlen + 1, cmd, clen + 1);

	return (full);
}

/**
 * search_path - search cmd in PATH (treat empty element as ".")
 * @cmd: command name (no '/')
 * @path_env: PATH value
 * Return: malloc'ed full path if found/executable, else NULL (sets errno)
 */
static char *search_path(const char *cmd, const char *path_env)
{
	char *copy, *dir, *full;

	copy = malloc(strlen(path_env) + 1);
	if (!copy)
		return (NULL);
	memcpy(copy, path_env, strlen(path_env) + 1);

	dir = strtok(copy, ":");
	while (dir)
	{
		/* empty PATH element => current directory */
		if (*dir == '\0')
			dir = ".";

		full = build_candidate(dir, cmd);
		if (!full)
		{
			free(copy);
			return (NULL);
		}
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
 * resolve_cmd - find executable for cmd via PATH or direct path
 * @cmd: argv[0]
 * Return: malloc'ed full path on success, or NULL if not found
 */
char *resolve_cmd(const char *cmd)
{
	size_t i;
	const char *path_env;

	if (!cmd || *cmd == '\0')
		return (NULL);

	for (i = 0; cmd[i]; i++)
	{
		if (cmd[i] == '/')
			return (try_direct(cmd));
	}

	path_env = get_path_value();
	if (!path_env || *path_env == '\0')
	{
		errno = ENOENT;
		return (NULL);
	}
	return (search_path(cmd, path_env));
}
