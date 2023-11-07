#include "my_shell.h"

/**
 * custom_show_env - Print the current environment variables.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_show_env(custom_shell_info_t *info)
{
    print_environment_variables(info->environment);
    return (0);
}

/**
 * find_env_variable - Get the value of an environment variable by name.
 * @info: Structure containing potential arguments and shell information.
 * @name: The name of the environment variable.
 * 
 * @Return: The value of the environment variable, or NULL if not found.
 */
char *find_env_variable(custom_shell_info_t *info, const char *name)
{
    list_t *node = info->environment;
    char *value;

    while (node)
    {
        value = starts_with(node->str, name);
        if (value && *value)
            return value;
        node = node->next;
    }
    return (NULL);
}

/**
 * custom_set_env - Set or modify an environment variable.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_set_env(custom_shell_info_t *info)
{
    if (info->argc != 3)
    {
        _eputs("Incorrect number of arguments\n");
        return (1);
    }
    if (set_environment_variable(info, info->argv[1], info->argv[2]))
        return (0);
    return (1);
}

/**
 * custom_unset_env - Remove one or more environment variables.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_unset_env(custom_shell_info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        _eputs("Too few arguments.\n");
        return (1);
    }

    for (i = 1; i <= info->argc; i++)
    {
        unset_environment_variable(info, info->argv[i]);
    }

    return (0);
}

/**
 * populate_environment_list - Populate the environment variable linked list.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int populate_environment_list(custom_shell_info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
    {
        add_node_end(&node, environ[i], 0);
    }
    info->environment = node;
    return (0);
}
