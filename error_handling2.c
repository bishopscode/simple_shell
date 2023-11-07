#include "my_shell.h"

/**
 * custom_erratoi - Convert a string to an integer.
 * @str: The string to be converted.
 * 
 * @Return: The converted integer, or -1 on error.
 */
int custom_erratoi(char *str)
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
 * custom_print_error - Print an error message.
 * @info: The parameter and return information structure.
 * @error_message: The error message to print.
 */
void custom_print_error(custom_shell_info_t *info, char *error_message)
{
    custom_eputs(info->program_name);
    custom_eputs(": ");
    custom_print_d(info->line_number, STDERR_FILENO);
    custom_eputs(": ");
    custom_eputs(info->command_name);
    custom_eputs(": ");
    custom_eputs(error_message);
}

/**
 * custom_print_d - Write an integer to the given file descriptor.
 * @integer: The integer to be written.
 * @fd: The file descriptor to write to.
 */
int custom_print_d(int integer, int fd)
{
	int (*__putchar)(char) = custom_putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = custom_eputchar;
	if (integer < 0)
	{
		_abs_ = -integer;
		__putchar('-');
		count++;
	}
	else
		_abs_ = integer;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}
/**
 * custom_convert_numbers - Convert a number to a string in the given base.
 * @number: The number to be converted.
 * @base: The base for conversion.
 * @flags: Flags for conversion.
 * 
 * @Return: The converted string.
 */
char *custom_convert_numbers(long int number, int base, int flags) 
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = number;

	if (!(flags & CONVERT_UNSIGNED) && number < 0)
	{
		n = -number;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	
    {
		*--ptr = array[n % base];
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
