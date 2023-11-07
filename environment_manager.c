#include "my_shell.h"

/**
 * custom_myenv - Print the current environment variables.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_myenv(custom_shell_info_t *info)
{
    print_list_str(info->env);
    return (0);
}

/**
 * custom_getenv - Get the value of an environment variable by name.
 * @info: Structure containing potential arguments and shell information.
 * @name: The name of the environment variable.
 * 
 * @Return: The value of the environment variable, or NULL if not found.
 */
char *custom_getenv(custom_shell_info_t *info, const char *name)
{
    list_t *node = info->env;
    char *p;

    while (node)
    {
        p = starts_with(node->str, name);
        if (p && *p)
            return (p);
        node = node->next;
    }
    return (NULL);
}

/**
 * custom_mysetenv - Set or modify an environment variable.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_mysetenv(custom_shell_info_t *info)
{
    if (info->argc != 3)
    {
        custom_eputs("Incorrect number of arguments\n");
        return (1);
    }
    if (custom_setenv(info, info->argv[1], info->argv[2]))
        return (0);
    return (1);
}

/**
 * custom_myunsetenv - Remove one or more environment variables.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int custom_myunsetenv(custom_shell_info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        custom_eputs("Too few arguments.\n");
        return (1);
    }

    for (i = 1; i <= info->argc; i++)
    {
        custom_unsetenv(info, info->argv[i]);
    }

    return (0);
}

/**
 * populate_environment_list - Populate the environment variable linked list.
 * @info: Structure containing potential arguments and shell information.
 * 
 * @Return: Always 0
 */
int populate_env_list(custom_shell_info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
    {
        add_node_end(&node, environ[i], 0);
    }
    info->env = node;
    return (0);
}
