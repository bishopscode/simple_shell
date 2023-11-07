#include "my_shell.h"

/**
 * custom_is_chain - test if current char in buffer is a custom chain delimiter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of current position in buffer
 *
 * Return: 1 if custom chain delimiter, 0 otherwise
 */
int custom_is_chain(custom_shell_info_t *info, char *buffer, size_t *position)
{
	size_t j = *position;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*position = j;
	return (1);
}

/**
 * custom_check_chain - checks whether we should continue chaining based on last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of current position in buffer
 * @start_pos: starting position in buffer
 * @length: length of buffer
 *
 * Return: Void
 */
void custom_check_chain(custom_shell_info_t *info, char *buffer, size_t *position, size_t start_pos, size_t length)
{
	size_t j = *position;

	if (info->cmd_buf_type == CUSTOM_CMD_AND)
	{
		if (info->status)
		{
			buffer[start_pos] = 0;
			j = length;
		}
	}
	if (info->cmd_buf_type == CUSTOM_CMD_OR)
	{
		if (!info->status)
		{
			buffer[start_pos] = 0;
			j = length;
		}
	}

	*position = j;
}

/**
 * custom_replace_alias - replaces custom aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int custom_replace_alias(custom_shell_info_t *info)
{
	int i;
	list_t *node;
	char *alias_value;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		alias_value = custom_strdup(custom_strchr(node->str, '=') + 1);
		if (!alias_value)
			return (0);
		info->argv[0] = alias_value;
	}
	return (1);
}

/**
 * custom_replace_vars - replaces custom variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int custom_replace_vars(custom_shell_info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (custom_strcmp(info->argv[i], "$?") == 0)
		{
			custom_replace_string(&(info->argv[i]),
				custom_strdup(custom_convert_number(info->status, 10, 0)));
			continue;
		}
		if (custom_strcmp(info->argv[i], "$$") == 0)
		{
			custom_replace_string(&(info->argv[i]),
				custom_strdup(custom_convert_number(getpid(), 10, 0)));
			continue;
		}
		node = custom_node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			custom_replace_string(&(info->argv[i]),
				custom_strdup(custom_strchr(node->str, '=') + 1));
			continue;
		}
		custom_replace_string(&info->argv[i], custom_strdup(""));

	}
	return (0);
}

/**
 * custom_replace_string - replaces a string
 * @old_string: address of old string
 * @new_string: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int custom_replace_string(char **old_string, char *new_string)
{
	custom_free(*old_string);
	*old_string = new_string;
	return (1);
}
