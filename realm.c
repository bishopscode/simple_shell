#include "my_shell.h"

/**
 * custom_memset - Fills memory with a constant byte.
 * @memory: The pointer to the memory area.
 * @byte: The byte to fill the memory with.
 * @size: The number of bytes to be filled.
 *
 * Return: (memory) a pointer to the filled memory area.
 */
char *custom_memset(char *memory, char byte, unsigned int size)
{
    unsigned int i;

    for (i = 0; i < size; i++)
        memory[i] = byte;
    return (memory);
}

/**
 * custom_ffree - Frees an array of strings.
 * @str_array: The array of strings to free.
 */
void custom_ffree(char **str_array)
{
    char **strings = str_array;

    if (!str_array)
        return;
    while (*str_array)
        free(*str_array++);
    free(strings);
}

/**
 * custom_realloc - Reallocates a block of memory.
 * @old_memory: Pointer to the previously allocated block.
 * @old_size: Size of the previous block in bytes.
 * @new_size: Size of the new block in bytes.
 *
 * Return: Pointer to the new block of memory.
 */
void *custom_realloc(void *old_memory, unsigned int old_size, unsigned int new_size)
{
    char *new_memory;

    if (!old_memory)
        return (malloc(new_size));
    if (!new_size)
        return (free(old_memory), NULL);
    if (new_size == old_size)
        return (old_memory);

    new_memory = malloc(new_size);
    if (!new_memory)
        return (NULL);

    old_size = old_size < new_size ? old_size : new_size;
    while (old_size--)
        new_memory[old_size] = ((char *)old_memory)[old_size];
    free(old_memory);
    return (new_memory);
}
