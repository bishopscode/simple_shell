#include "my_shell.h"

/**
 * custom_list_length - Determines the length of a custom linked list.
 * @h: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t custom_list_length(const custom_list_t *h)
{
    size_t size = 0;

    while (h)
    {
        h = h->next;
        size++;
    }
    return (size);
}

/**
 * custom_list_to_strings - Returns an array of strings from the custom list.
 * @head: Pointer to the first node.
 *
 * Return: Array of strings.
 */
char **custom_list_to_strings(custom_list_t *head)
{
    custom_list_t *node = head;
    size_t i = custom_list_length(head), j;
    char **strings;
    char *str;

    if (!head || !i)
        return (NULL);

    strings = custom_malloc(sizeof(char *) * (i + 1));

    if (!strings)
        return (NULL);

    for (i = 0; node; node = node->next, i++)
    {
        str = custom_malloc(custom_strlen(node->data) + 1);

        if (!str)
        {
            for (j = 0; j < i; j++)
                custom_free(strings[j]);
            custom_free(strings);
            return (NULL);
        }

        str = custom_strcpy(str, node->data);
        strings[i] = str;
    }

    strings[i] = NULL;
    return (strings);
}

/**
 * custom_print_custom_list - Prints all elements of a custom linked list.
 * @h: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t custom_print_custom_list(const custom_list_t *h)
{
    size_t size = 0;

    while (h)
    {
        custom_puts(custom_convert_number(h->index, 10, 0));
        custom_putchar(':');
        custom_putchar(' ');
        custom_puts(h->data ? h->data : "(nil)");
        custom_puts("\n");
        h = h->next;
        size++;
    }
    return (size);
}

/**
 * custom_node_starts_with - Returns the node whose string starts with a prefix.
 * @node: Pointer to the custom list head.
 * @prefix: String to match.
 * @c: The next character after the prefix to match.
 *
 * Return: Matching node or NULL.
 */
custom_list_t *custom_node_starts_with(custom_list_t *node, char *prefix, char c)
{
    char *p = NULL;

    while (node)
    {
        p = custom_starts_with(node->data, prefix);
        if (p && ((c == -1) || (*p == c)))
            return (node);
        node = node->next;
    }
    return (NULL);
}

/**
 * custom_get_node_index - Gets the index of a node.
 * @head: Pointer to the custom list head.
 * @node: Pointer to the node.
 *
 * Return: Index of the node or -1.
 */
ssize_t custom_get_node_index(custom_list_t *head, custom_list_t *node)
{
    size_t index = 0;

    while (head)
    {
        if (head == node)
            return (index);
        head = head->next;
        index++;
    }
    return (-1);
}
