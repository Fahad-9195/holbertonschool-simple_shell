#include "shell.h"

/**
 * sanitize_line - trim newline and outer spaces/tabs in-place
 * @s: line from getline
 * Return: pointer to first non-space char (or NULL)
 */
char *sanitize_line(char *s)
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
 * build_argv - split by spaces/tabs (no quotes/escapes)
 * @line: sanitized line, tokenized in-place
 * @argv: output argv buffer
 * @max: max argv entries
 * Return: argc
 */
int build_argv(char *line, char **argv, size_t max)
{
	size_t i = 0;
	char *tok;

	if (!line || !*line)
		return (0);

	tok = strtok(line, " \t");
	while (tok && i + 1 < max)
	{
		argv[i++] = tok;
		tok = strtok(NULL, " \t");
	}
	argv[i] = NULL;
	return ((int)i);
}
