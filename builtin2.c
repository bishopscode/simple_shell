#include "my_shell.h"

/**
 * custom_myhistory - Display the command history with line numbers.
 * @info: Structure containing potential arguments and shell information.
 *
 * @Return: Always 0
 */
int custom_myhistory(custom_shell_info_t *info)
{
    custom_print_list(info->history);
    return (0);
}

/**
 * custom_unset_alias - Remove an alias from the alias list.
 * @info: Structure containing potential arguments and shell information.
 * @str: The name of the alias to remove.
 *
 * @Return: 0 on success, 1 on error
 */
int custom_unset_alias(custom_shell_info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = custom_strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}
/**
 * custom_set_alias - Add an alias to the alias list.
 * @info: Structure containing potential arguments and shell information.
 * @str: The alias string to add.
 * 
 * @Return: 0 on success, 1 on error
 */
int custom_set_alias(custom_shell_info_t *info, char *str)
{
	char *p;

	p = custom_strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	custom_unset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}
/**
 * custom_print_alias - Print a single alias from the alias list.
 * @node: The alias node to print.
 * 
 * @Return: 0 on success, 1 on error
 */
int custom_print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = custom_strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			custom_putchar(*a);
		custom_putchar('\'');
		custom_puts(p + 1);
		custom_puts("'\n");
		return (0);
	}
	return (1);
}
/**
 * custom_myalias - Manage aliases for the custom shell.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_myalias(custom_shell_info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			custom_print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = _strchr(info->argv[i], '=');
		if (p)
			custom_set_alias(info, info->argv[i]);
		else
			custom_print_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}