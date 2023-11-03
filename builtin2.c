#include "my_shell.h"

/**
 * custom_history - Display the command history with line numbers.
 * @info: Structure containing potential arguments and shell information.
 *
 * @Return: Always 0
 */
int custom_history(custom_shell_info_t *info)
{
    display_command_history(info->history);
    return (0);
}

/**
 * remove_alias - Remove an alias from the alias list.
 * @info: Structure containing potential arguments and shell information.
 * @alias_name: The name of the alias to remove.
 *
 * @Return: 0 on success, 1 on error
 */
int remove_alias(custom_shell_info_t *info, char *alias_name)
{
    char *equal_sign, tmp_char;
    int result;

    equal_sign = _strchr(alias_name, '=');
    if (!equal_sign)
        return (1);
    
    tmp_char = *equal_sign;
    *equal_sign = '\0';
    result = delete_alias_node(info, alias_name);
    *equal_sign = tmp_char;
    
    return (result);
}

/**
 * add_alias - Add an alias to the alias list.
 * @info: Structure containing potential arguments and shell information.
 * @alias_str: The alias string to add.
 * 
 * @Return: 0 on success, 1 on error
 */
int add_alias(custom_shell_info_t *info, char *alias_str)
{
    char *equal_sign;

    equal_sign = _strchr(alias_str, '=');
    if (!equal_sign)
        return (1);
    
    if (!*++equal_sign)
        return (remove_alias(info, alias_str));
    
    remove_alias(info, alias_str);
    return (add_alias_node(&(info->aliases), alias_str) == NULL);
}

/**
 * print_single_alias - Print a single alias from the alias list.
 * @alias_node: The alias node to print.
 * 
 * @Return: 0 on success, 1 on error
 */
int print_single_alias(list_t *alias_node)
{
    char *equal_sign = NULL, *alias_value = NULL;

    if (alias_node)
    {
        equal_sign = _strchr(alias_node->str, '=');
        for (alias_value = alias_node->str; alias_value <= equal_sign; alias_value++)
            _putchar(*alias_value);
        _putchar('\'');
        _puts(equal_sign + 1);
        _puts("'\n");
        return (0);
    }
    return (1);
}

/**
 * custom_alias - Manage aliases for the custom shell.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_alias(custom_shell_info_t *info)
{
    int i = 0;
    char *equal_sign = NULL;
    list_t *alias_node = NULL;

    if (info->argc == 1)
    {
        alias_node = info->aliases;
        while (alias_node)
        {
            print_single_alias(alias_node);
            alias_node = alias_node->next;
        }
        return (0);
    }

    for (i = 1; info->argv[i]; i++)
    {
        equal_sign = _strchr(info->argv[i], '=');
        if (equal_sign)
        {
            add_alias(info, info->argv[i]);
        } else
        {
            print_single_alias(find_alias_with_prefix(info->aliases, info->argv[i], '='));
        }
    }

    return (0);
}
