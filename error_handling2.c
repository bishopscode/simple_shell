#include "my_shell.h"

/**
 * custom_str_to_int - Convert a string to an integer.
 * @str: The string to be converted.
 * 
 * @Return: The converted integer, or -1 on error.
 */
int custom_str_to_int(char *str)
{
    int index = 0;
    unsigned long int result = 0;

    if (*str == '+')
        str++;

    for (index = 0; str[index] != '\0'; index++)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            result *= 10;
            result += (str[index] - '0');
            if (result > INT_MAX)
                return (-1);
        } else 
        {
            return (-1);
        }
    }

    return (result);
}

/**
 * custom_print_error_message - Print an error message.
 * @info: The parameter and return information structure.
 * @error_message: The error message to print.
 */
void custom_print_error_message(custom_shell_info_t *info, char *error_message)
{
    custom_write_string(info->program_name);
    custom_write_string(": ");
    custom_write_integer(info->line_number, STDERR_FILENO);
    custom_write_string(": ");
    custom_write_string(info->command_name);
    custom_write_string(": ");
    custom_write_string(error_message);
}

/**
 * custom_write_integer - Write an integer to the given file descriptor.
 * @integer: The integer to be written.
 * @fd: The file descriptor to write to.
 */
void custom_write_integer(int integer, int fd)
{
    char character_buffer[ERROR_BUFFER_SIZE];
    int buffer_index = 0;

    if (integer < 0)
    {
        character_buffer[buffer_index++] = '-';
        integer = -integer;
    }

    if (integer == 0)
    {
        character_buffer[buffer_index++] = '0';
    } else
    {
        while (integer > 0)
        {
            character_buffer[buffer_index++] = '0' + (integer % 10);
            integer /= 10;
        }
    }

    for (buffer_index--; buffer_index >= 0; buffer_index--)
    {
        custom_write_character(character_buffer[buffer_index], fd);
    }
}

/**
 * custom_convert_to_string - Convert a number to a string in the given base.
 * @number: The number to be converted.
 * @base: The base for conversion.
 * @flags: Flags for conversion.
 * 
 * @Return: The converted string.
 */
char *custom_convert_to_string(long int number, int base, int flags) 
{
    static char conversion_array[CONVERSION_BUFFER_SIZE];
    static char conversion_buffer[CONVERSION_BUFFER_SIZE];
    char sign = 0;
    char *ptr;
    unsigned long n = number;

    if (!(flags & CONVERT_UNSIGNED) && number < 0)
    {
        n = -number;
        sign = '-';
    }

    ptr = &conversion_buffer[CONVERSION_BUFFER_SIZE - 1];
    *ptr = '\0';

    do
    {
        *--ptr = conversion_array[n % base];
        n /= base;
    } while (n != 0);

    if (sign)
        *--ptr = sign;

    return (ptr);
}

/**
 * custom_remove_comments - Replace the first '#' character with '\0' in the string.
 * @buffer: The address of the string to modify.
 */
void custom_remove_comments(char *buffer)
{
    int index;

    for (index = 0; buffer[index] != '\0'; index++)
    {
        if (buffer[index] == '#' && (!index || buffer[index - 1] == ' '))
        {
            buffer[index] = '\0';
            break;
        }
    }
}
