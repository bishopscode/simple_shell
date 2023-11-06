#include "my_shell.h"

/**
 * custom_tokenizer - splits a string into words. Repeated delimiters are ignored
 * @input_string: the input string
 * @delimiter_string: the delimiter string
 *
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **custom_tokenizer(char *input_string, char *delimiter_string)
{
	int i, j, k, m, num_tokens = 0;
	char **tokens;

	if (input_string == NULL || input_string[0] == 0)
		return (NULL);
	if (!delimiter_string)
		delimiter_string = " ";
	for (i = 0; input_string[i] != '\0'; i++)
		if (!is_custom_delimiter(input_string[i], delimiter_string) &&
			(is_custom_delimiter(input_string[i + 1], delimiter_string) || !input_string[i + 1]))
			num_tokens++;

	if (num_tokens == 0)
		return (NULL);
	tokens = malloc((1 + num_tokens) * sizeof(char *));
	if (!tokens)
		return (NULL);
	for (i = 0, j = 0; j < num_tokens; j++)
	{
		while (is_custom_delimiter(input_string[i], delimiter_string))
			i++;
		k = 0;
		while (!is_custom_delimiter(input_string[i + k], delimiter_string) && input_string[i + k])
			k++;
		tokens[j] = malloc((k + 1) * sizeof(char));
		if (!tokens[j])
		{
			for (k = 0; k < j; k++)
				free(tokens[k]);
			free(tokens);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			tokens[j][m] = input_string[i++];
		tokens[j][m] = 0;
	}
	tokens[j] = NULL;
	return (tokens);
}

/**
 * custom_tokenizer_v2 - splits a string into words using a specified delimiter
 * @input_string: the input string
 * @delimiter: the delimiter character
 *
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **custom_tokenizer_v2(char *input_string, char delimiter)
{
	int i, j, k, m, num_tokens = 0;
	char **tokens;

	if (input_string == NULL || input_string[0] == 0)
		return (NULL);
	for (i = 0; input_string[i] != '\0'; i++)
		if ((input_string[i] != delimiter && input_string[i + 1] == delimiter) ||
			(input_string[i] != delimiter && !input_string[i + 1]) || input_string[i + 1] == delimiter)
			num_tokens++;

	if (num_tokens == 0)
		return (NULL);
	tokens = malloc((1 + num_tokens) * sizeof(char *));
	if (!tokens)
		return (NULL);
	for (i = 0, j = 0; j < num_tokens; j++)
	{
		while (input_string[i] == delimiter && input_string[i] != delimiter)
			i++;
		k = 0;
		while (input_string[i + k] != delimiter && input_string[i + k] && input_string[i + k] != delimiter)
			k++;
		tokens[j] = malloc((k + 1) * sizeof(char));
		if (!tokens[j])
		{
			for (k = 0; k < j; k++)
				free(tokens[k]);
			free(tokens);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			tokens[j][m] = input_string[i++];
		tokens[j][m] = 0;
	}
	tokens[j] = NULL;
	return (tokens);
}
