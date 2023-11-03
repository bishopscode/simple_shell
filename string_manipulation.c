#include "my_shell.h"

/**
 * custom_copy_string - Copy a string to another string with a specified length.
 * @destination: The destination string.
 * @source: The source string.
 * @length: The maximum number of characters to copy.
 * 
 * @Return: The pointer to the destination string.
 */
char *custom_copy_string(char *destination, char *source, int length)
{
    int source_index, destination_index;
    char *result = destination;

    source_index = 0;
    while (source[source_index] != '\0' && source_index < length - 1)
    {
        destination[destination_index] = source[source_index];
        source_index++;
        destination_index++;
    }

    if (source_index < length)
    {
        int padding_index = source_index;
        while (padding_index < length)
        {
            destination[padding_index] = '\0';
            padding_index++;
        }
    }
    
    return (result);
}

/**
 * custom_concatenate_strings - Concatenate two strings with a specified length.
 * @first_string: The first string.
 * @second_string: The second string.
 * @length: The maximum number of bytes to use.
 * 
 * @Return: The pointer to the concatenated string.
 */
char *custom_concatenate_strings(char *first_string, char *second_string, int length)
{
    int first_index, second_index;
    char *result = first_string;

    first_index = 0;
    while (first_string[first_index] != '\0')
    {
        first_index++;
    }

    second_index = 0;
    while (second_string[second_index] != '\0' && second_index < length)
    {
        first_string[first_index] = second_string[second_index];
        first_index++;
        second_index++;
    }

    if (second_index < length)
    {
        first_string[first_index] = '\0';
    }
    
    return (result);
}

/**
 * custom_find_character - Locate a character in a string.
 * @string: The string to be searched.
 * @character: The character to look for.
 * 
 * @Return: A pointer to the first occurrence of the character in the string.
 */
char *custom_find_character(char *string, char character)
{
    do
    {
        if (*string == character) 
        {
            return (string);
        }
    } while (*string++ != '\0');

    return (NULL);
}
