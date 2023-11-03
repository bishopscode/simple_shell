#include "my_shell.h"

/**
 * custom_get_environment - Get a copy of the environment variables as a string array.
 * @info: The shell information and arguments.
 * 
 * @Return: A copy of the environment variables.
 */
char **custom_get_environment(custom_shell_info_t *info)
{
    if (!info->custom_environment || info->environment_changed)
    {
        info->custom_environment = list_to_strings(info->custom_environment_list);
        info->environment_changed = 0;
    }

    return (info->custom_environment);
}

/**
 * custom_unset_environment_variable - Remove an environment variable.
 * @info: The shell information and arguments.
 * @variable: The name of the environment variable to remove.
 * 
 * @Return: 1 if the variable is deleted, 0 otherwise.
 */
int custom_unset_environment_variable(custom_shell_info_t *info, char *variable)
{
    list_t *node = info->custom_environment_list;
    size_t index = 0;
    char *property;

    if (!node || !variable)
    {
        return (0);
    }

    while (node)
    {
        property = starts_with(node->str, variable);
        if (property && *property == '=')
        {
            info->environment_changed = delete_node_at_index(&(info->custom_environment_list), index);
            index = 0;
            node = info->custom_environment_list;
            continue;
        }
        node = node->next;
        index++;
    }
    return (info->environment_changed);
}

/**
 * custom_set_environment_variable - Initialize or modify an environment variable.
 * @info: The shell information and arguments.
 * @variable: The name of the environment variable.
 * @value: The value of the environment variable.
 * 
 * @Return: 0 on success, 1 on failure.
 */
int custom_set_environment_variable(custom_shell_info_t *info, char *variable, char *value)
{
    char *buffer = NULL;
    list_t *node;
    char *property;

    if (!variable || !value)
    {
        return (0);
    }

    buffer = malloc(custom_strlen(variable) + custom_strlen(value) + 2);
    if (!buffer)
    {
        return (1);
    }
    custom_strcpy(buffer, variable);
    custom_strcat(buffer, "=");
    custom_strcat(buffer, value);
    node = info->custom_environment_list;
    while (node)
    {
        property = starts_with(node->str, variable);
        if (property && *property == '=')
        {
            free(node->str);
            node->str = buffer;
            info->environment_changed = 1;
            return (0);
        }
        node = node->next;
    }
    add_node_end(&(info->custom_environment_list), buffer, 0);
    free(buffer);
    info->environment_changed = 1;
    return (0);
}
