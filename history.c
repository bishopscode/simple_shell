#include "my_shell.h"

/**
 * custom_get_history_filepath - Get the custom history file path.
 * @info: Custom shell information struct.
 *
 * Return: Allocated string containing the history file path.
 */
char *custom_get_history_filepath(custom_shell_info_t *info)
{
    char *home_dir, *file_path;

    home_dir = custom_getenv(info, "HOME=");
    if (!home_dir)
        return (NULL);

    file_path = custom_malloc(sizeof(char) * (custom_strlen(home_dir) + custom_strlen(CUSTOM_HISTORY_FILE) + 2));
    if (!file_path)
        return (NULL);

    file_path[0] = 0;
    custom_strcpy(file_path, home_dir);
    custom_strcat(file_path, "/");
    custom_strcat(file_path, CUSTOM_HISTORY_FILE);
    return (file_path);
}

/**
 * custom_write_history - Create a file or append to an existing file for custom shell history.
 * @info: Custom shell information struct.
 *
 * Return: 1 on success, -1 on failure.
 */
int custom_write_history(custom_shell_info_t *info)
{
    ssize_t fd;
    char *filename = custom_get_history_filepath(info);
    custom_list_t *node = NULL;

    if (!filename)
        return (-1);

    fd = custom_open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    custom_free(filename);

    if (fd == -1)
        return (-1);

    for (node = info->custom_history; node; node = node->next)
    {
        custom_putsfd(node->str, fd);
        custom_putfd('\n', fd);
    }

    custom_putfd(CUSTOM_BUF_FLUSH, fd);
    custom_close(fd);

    return (1);
}

/**
 * custom_read_history - Read history from the custom shell history file.
 * @info: Custom shell information struct.
 *
 * Return: The new history count on success, 0 on failure.
 */
int custom_read_history(custom_shell_info_t *info)
{
    int i, last = 0, line_count = 0;
    ssize_t fd, read_len, file_size = 0;
    struct stat st;
    char *buf = NULL, *filename = custom_get_history_filepath(info);

    if (!filename)
        return (0);

    fd = custom_open(filename, O_RDONLY);
    custom_free(filename);

    if (fd == -1)
        return (0);

    if (!custom_fstat(fd, &st))
        file_size = st.st_size;

    if (file_size < 2)
        return (0);

    buf = custom_malloc(sizeof(char) * (file_size + 1));

    if (!buf)
        return (0);

    read_len = custom_read(fd, buf, file_size);
    buf[file_size] = 0;

    if (read_len <= 0)
        return (custom_free(buf), 0);

    custom_close(fd);

    for (i = 0; i < file_size; i++) 
    {
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            custom_build_history_list(info, buf + last, line_count++);
            last = i + 1;
        }
    }

    if (last != i)
        custom_build_history_list(info, buf + last, line_count++);

    custom_free(buf);
    info->custom_hist_count = line_count;

    while (info->custom_hist_count-- >= CUSTOM_HIST_MAX)
    {
        custom_delete_node_at_index(&(info->custom_history), 0);
    }

    custom_renumber_history(info);

    return (info->custom_hist_count);
}

/**
 * custom_build_history_list - Add an entry to the custom shell history linked list.
 * @info: Custom shell information struct.
 * @buf: Buffer to add to the history.
 * @line_count: The history line count.
 *
 * Return: 0 always.
 */
int custom_build_history_list(custom_shell_info_t *info, char *buf, int line_count)
{
    custom_list_t *node = NULL;

    if (info->custom_history)
        node = info->custom_history;

    custom_add_node_end(&node, buf, line_count);

    if (!info->custom_history)
        info->custom_history = node;

    return (0);
}

/**
 * custom_renumber_history - Renumber the history linked list after changes.
 * @info: Custom shell information struct.
 *
 * Return: The new history count.
 */
int custom_renumber_history(custom_shell_info_t *info)
{
    custom_list_t *node = info->custom_history;
    int i = 0;

    while (node) {
        node->num = i++;
        node = node->next;
    }

    info->custom_hist_count = i;
    return (info->custom_hist_count);
}
