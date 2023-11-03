#include "custom_shell.h"

/**
 * custom_input_buffer - Buffers chained commands in custom shell.
 * @info: Custom shell information struct.
 * @buffer: Address of buffer to store the input.
 * @buffer_length: Address of buffer length variable.
 *
 * Return: Bytes read.
 */
ssize_t custom_input_buffer(custom_shell_info_t *info, char **buffer, size_t *buffer_length)
{
    ssize_t bytes_read = 0;
    size_t current_length = 0;

    if (!*buffer_length)
    {
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, custom_sigint_handler);
        #if USE_GETLINE
        bytes_read = getline(buffer, &current_length, stdin);
        #else
        bytes_read = custom_getline(info, buffer, &current_length);
        #endif

        if (bytes_read > 0) 
        {
            if ((*buffer)[bytes_read - 1] == '\n')
            {
                (*buffer)[bytes_read - 1] = '\0'; 
                bytes_read--;
            }

            info->linecount_flag = 1;
            custom_remove_comments(*buffer);
            custom_build_history_list(info, *buffer, info->histcount++);
            {
                *buffer_length = bytes_read;
                info->cmd_buffer = buffer;
            }
        }
    }

    return (bytes_read);
}

/**
 * custom_get_input - Gets a line of input without the newline character.
 * @info: Custom shell information struct.
 *
 * Return: Bytes read.
 */
ssize_t custom_get_input(custom_shell_info_t *info)
{
    static char *buffer;
    static size_t current_position, next_position, buffer_length;
    ssize_t bytes_read = 0;
    char **input_buffer_pointer = &(info->custom_arg);
    char *position;

    custom_putchar(CUSTOM_BUF_FLUSH);
    bytes_read = custom_input_buffer(info, &buffer, &buffer_length);

    if (bytes_read == -1)
    {
        return (-1);
    }

    if (buffer_length)
    {
        /* We have commands left in the chain buffer.*/
        next_position = current_position;
        position = buffer + current_position;

        custom_check_chain(info, buffer, &next_position, current_position, buffer_length);

        while (next_position < buffer_length) 
        {
            if (custom_is_chain(info, buffer, &next_position))
            {
                break;
            }
            next_position++;
        }

        current_position = next_position + 1;

        if (current_position >= buffer_length)
        {
            current_position = buffer_length = 0;
            info->cmd_buffer_type = CMD_NORMAL;
        }

        *input_buffer_pointer = position;
        return (custom_strlen(position));
    }

    *input_buffer_pointer = buffer;
    return (bytes_read);
}

/**
 * custom_read_buffer - Reads a buffer.
 * @info: Custom shell information struct.
 * @buffer: Buffer to read.
 * @length: Size.
 *
 * Return: Bytes read.
 */
ssize_t custom_read_buffer(custom_shell_info_t *info, char *buffer, size_t *length)
{
    ssize_t bytes_read = 0;

    if (*length)
    {
        return (0);
    }

    bytes_read = read(info->custom_readfd, buffer, CUSTOM_READ_BUF_SIZE);
    if (bytes_read >= 0)
    {
        *length = bytes_read;
    }

    return (bytes_read);
}

/**
 * custom_getline - Gets the next line of input from STDIN.
 * @info: Custom shell information struct.
 * @pointer: Address of a pointer to the buffer (preallocated or NULL).
 * @length: Size of the preallocated pointer buffer if not NULL.
 *
 * Return: Size.
 */
int custom_getline(custom_shell_info_t *info, char **pointer, size_t *length)
{
    static char buffer[CUSTOM_READ_BUF_SIZE];
    static size_t current_position, length;
    size_t k;
    ssize_t bytes_read = 0;
    ssize_t size = 0;
    char *position = NULL, *new_position = NULL, *character;

    position = *pointer;

    if (position && length)
    {
        size = *length;
    }

    if (current_position == length)
    {
        current_position = length = 0;
    }

    bytes_read = custom_read_buffer(info, buffer, &length);
    if (bytes_read == -1 || (bytes_read == 0 && length == 0))
    {
        return (-1);
    }

    character = custom_strchr(buffer + current_position, '\n');
    k = character ? 1 + (unsigned int)(character - buffer) : length;
    new_position = custom_realloc(position, size, size ? size + k : k + 1);

    if (!new_position)
    {
        return (position ? free(position), -1 : -1);
    }

    if (size)
    {
        custom_strncat(new_position, buffer + current_position, k - current_position);
    } else
    {
        custom_strncpy(new_position, buffer + current_position, k - current_position + 1);
    }

    size += k - current_position;
    current_position = k;
    position = new_position;

    if (length) {
        *length = size;
    }

    *pointer = position;
    return (size);
}

/**
 * custom_sigint_handler - Blocks Ctrl-C signal.
 * @sig_num: Signal number.
 *
 * Return: void.
 */
void custom_sigint_handler(__attribute__((unused)) int sig_num)
{
    custom_puts("\n");
    custom_puts("$ ");
    custom_putchar(CUSTOM_BUF_FLUSH);
}
