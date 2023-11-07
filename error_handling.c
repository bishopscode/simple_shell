#include "my_shell.h"

/**
 * custom_print_error - Print an error message to the standard error.
 * @error_message: The error message to be printed.
 */
void custom_print_error(char *error_message)
{
    int index = 0;

    if (!error_message)
        return;

    while (error_message[index] != '\0')
    {
        custom_write_error_char(error_message[index]);
        index++;
    }
}

/**
 * custom_write_error_char - Write a character to the standard error.
 * @param character: The character to be written.
 * 
 * @Return: 1 on success, -1 on error.
 */
int custom_write_error_char(char character)
{
    static int buffer_index;
    static char error_buffer[ERROR_WRITE_BUFFER_SIZE];

    if (character == ERROR_BUFFER_FLUSH || buffer_index >= ERROR_WRITE_BUFFER_SIZE)
    {
        write(STDERR_FILENO, error_buffer, buffer_index);
        buffer_index = 0;
    }

    if (character != ERROR_BUFFER_FLUSH)
        error_buffer[buffer_index++] = character;

    return (1);
}

/**
 * custom_write_to_fd - Write a character to a given file descriptor.
 * @character: The character to be written.
 * @fd: The file descriptor to write to.
 * 
 * @Return: 1 on success, -1 on error.
 */
int custom_write_to_fd(char character, int fd)
{
    static int buffer_index;
    static char write_buffer[ERROR_WRITE_BUFFER_SIZE];

    if (character == ERROR_BUFFER_FLUSH || buffer_index >= ERROR_WRITE_BUFFER_SIZE)
    {
        write(fd, write_buffer, buffer_index);
        buffer_index = 0;
    }

    if (character != ERROR_BUFFER_FLUSH)
        write_buffer[buffer_index++] = character;

    return (1);
}

/**
 * custom_puts_to_fd - Write a string to a given file descriptor.
 * @str: The string to be written.
 * @fd: The file descriptor to write to.
 * 
 * @Return The number of characters written.
 */
int custom_puts_to_fd(char *str, int fd)
{
    int char_count = 0;

    if (!str)
        return (0);

    while (*str)
    {
        char_count += custom_write_to_fd(*str++, fd);
    }

    return (char_count);
}
