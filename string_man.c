#include "my_shell.h"

/**
 * custom_strlen - returns the length of a string
 * @str: the string whose length to determine
 *
 * Return: the length of the string
 */
int custom_strlen(char *str)
{
	int length = 0;

	if (!str)
		return (0);

	while (*str)
	{
		length++;
		str++;
	}
	return (length);
}

/**
 * custom_strcmp - performs lexicographic comparison of two strings.
 * @str1: the first string
 * @str2: the second string
 *
 * Return: a negative value if str1 < str2, a positive value if str1 > str2, or 0 if str1 == str2
 */
int custom_strcmp(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
			return *str1 - *str2;
		str1++;
		str2++;
	}

	if (*str1 == *str2)
		return 0;
	else
		return (*str1 < *str2) ? -1 : 1;
}

/**
 * custom_starts_with - checks if a string starts with a given prefix
 * @haystack: the string to search
 * @needle: the prefix to find
 *
 * Return: a pointer to the character after the prefix in the string or NULL
 */
char *custom_starts_with(const char *haystack, const char *needle)
{
	while (*needle)
	{
		if (*needle++ != *haystack++)
			return (NULL);
	}

	return ((char *)haystack);
}

/**
 * custom_strcat - concatenates two strings
 * @destination: the destination buffer
 * @source: the source buffer
 *
 * Return: a pointer to the destination buffer
 */
char *custom_strcat(char *destination, char *source)
{
	char *result = destination;

	while (*destination)
		destination++;

	while (*source)
		*destination++ = *source++;

	*destination = *source;
	return (result);
}
