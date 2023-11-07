#include "my_shell.h"

/**
 * custom_is_cmd - Determines if a file is an executable command.
 * @info: The custom info struct.
 * @path: Path to the file.
 *
 * Return: 1 if it's an executable command, 0 otherwise.
 */
int custom_is_cmd(custom_shell_info_t *info, char *path)
{
    struct stat st;

    (void)info;
    if (!path || stat(path, &st))
        return (0);

    if (st.st_mode & S_IFREG)
        return (1);

    return (0);
}

/**
 * custom_dup_chars - Copies a substring of characters.
 * @source: The source string.
 * @start: Starting index.
 * @end: Ending index.
 *
 * Return: Pointer to a new buffer containing the copied substring.
 */
char *custom_dup_chars(char *source, int start, int end)
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
 * custom_find_path - Finds the full path of a command in the PATH string.
 * @info: The custom info struct.
 * @path_string: The PATH string.
 * @cmd: The command to find.
 *
 * Return: Full path of the command if found, or NULL if not found.
 */
char *custom_find_path(custom_shell_info_t *info, char *path_string, char *cmd)
{
    int i = 0, current_position = 0;
    char *full_path;

    if (!path_string)
        return (NULL);

    if ((custom_strlen(cmd) > 2) && starts_with(cmd, "./"))
    {
        if (custom_is_cmd(info, cmd))
            return (cmd);
    }

    while (1)
    {
        if (!path_string[i] || path_string[i] == ':')
        {
            full_path = custom_dup_chars(path_string, current_position, i);
            if (!*full_path)
                custom_strcat(full_path, cmd);
            else
            {
                custom_strcat(full_path, "/");
                custom_strcat(full_path, cmd);
            }
            if (custom_is_cmd(info, full_path))
                return (full_path);

            if (!path_string[i])
                break;
            current_position = i;
        }
        i++;
    }

    return (NULL);
}
