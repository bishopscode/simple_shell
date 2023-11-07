#include "my_shell.h"

/**
 * custom_is_executable - Determines if a file is an executable command.
 * @info: The custom info struct.
 * @file_path: Path to the file.
 *
 * Return: 1 if it's an executable command, 0 otherwise.
 */
int custom_is_executable(custom_info_t *info, char *file_path)
{
    struct stat file_stat;

    (void)info;
    if (!file_path || stat(file_path, &file_stat))
        return (0);

    if (file_stat.st_mode & S_IFREG)
        return (1);

    return (0);
}

/**
 * custom_copy_substring - Copies a substring of characters.
 * @source: The source string.
 * @start: Starting index.
 * @end: Ending index.
 *
 * Return: Pointer to a new buffer containing the copied substring.
 */
char *custom_copy_substring(char *source, int start, int end)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < end; i++)
    {
        if (source[i] != ':')
            buffer[k++] = source[i];
    }
    buffer[k] = 0;
    return (buffer);
}

/**
 * custom_find_executable_path - Finds the full path of a command in the PATH string.
 * @info: The custom info struct.
 * @path_string: The PATH string.
 * @command: The command to find.
 *
 * Return: Full path of the command if found, or NULL if not found.
 */
char *custom_find_executable_path(custom_info_t *info, char *path_string, char *command)
{
    int i = 0, current_position = 0;
    char *full_path;

    if (!path_string)
        return (NULL);

    if ((_strlen(command) > 2) && custom_starts_with(command, "./"))
    {
        if (custom_is_executable(info, command))
            return (command);
    }

    while (1)
    {
        if (!path_string[i] || path_string[i] == ':')
        {
            full_path = custom_copy_substring(path_string, current_position, i);
            if (!*full_path)
                custom_strcat(full_path, command);
            else
            {
                custom_strcat(full_path, "/");
                custom_strcat(full_path, command);
            }
            if (custom_is_executable(info, full_path))
                return (full_path);

            if (!path_string[i])
                break;
            current_position = i;
        }
        i++;
    }

    return (NULL);
}
