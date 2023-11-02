#include "my_shell.h"

/**
 * check_if_interactive - Determine if the shell is in interactive mode
 * @info: Pointer to the shell information structure
 *
 * Return: 1 if the shell is in interactive mode, 0 otherwise
 */
int check_if_interactive(custom_shell_info_t *info)
{
   /*
    *Checks if the std input is a term
    * and the read file descriptor is within the std range (0-2).
    */
	return (isatty(STDIN_FILENO) && info->read_fd <= 2);
}

/**
 * is_character_delimiter - Check if a character is a delimiter
 * @c: The character to check
 * @delimiters: The string containing delimiters
 *
 * Return: 1 if the character is a delimiter, 0 if not
 */
int is_character_delimiter(char c, char *delimiters)
{
	while (*delimiters)
	{
	if (*delimiters == c)
	{
		return (1); /*delimiter is found, return 1*/
	}
	delimiters++;
	}
	return (0);
}

/**
 * is_alphabetic - Check if a character is alphabetic
 * @c: The character to check
 *
 * Return: 1 if the character is alphabetic, 0 if not
 */
int is_alphabetic(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
	return (1);
	}
	return (0);
}

/**
 * custom_atoi - Convert a string to an integer
 * @s: The string to be converted
 *
 * Return: 0 if no numbers in the string, the converted number otherwise
 */
int custom_atoi(char *s)
{
	int i, sign = 1, flag = 0, result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
	if (s[i] == '-')
	{
		sign *= -1;
	}

	if (s[i] >= '0' && s[i] <= '9')
	{
		flag = 1;
		result *= 10;
		result += (s[i] - '0');
	}
	else if (flag == 1)
	{
		flag = 2;
	}
	}

	if (sign == -1)
	{
	return (-result);
	}
	else
	{
	return (result);
	}
}

