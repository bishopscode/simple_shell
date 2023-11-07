#include "my_shell.h"

/**
 * add_node - Add a node to the start of the custom list.
 * @head: Address of the pointer to the head node.
 * @data: Data field of the node.
 * @index: Node index used for history.
 *
 * Return: Pointer to the new head of the list.
 */
list_t *add_node(list_t **head, const char *data, int index)
{
    list_t *new_head;

    if (!head)
        return (NULL);

    new_head = malloc(sizeof(list_t));

    if (!new_head)
        return (NULL);

    custom_memset((void *)new_head, 0, sizeof(list_t));
    new_head->index = index;

    if (data)
    {
        new_head->data = custom_strdup(data);

        if (!new_head->data)
        {
            custom_free(new_head);
            return (NULL);
        }
    }

    new_head->next = *head;
    *head = new_head;
    return (new_head);
}

/**
 * add_node_end - Add a node to the end of the custom list.
 * @head: Address of the pointer to the head node.
 * @data: Data field of the node.
 * @index: Node index used for history.
 *
 * Return: Pointer to the new end of the list.
 */
list_t *add_node_end(list_t **head, const char *data, int index)
{
    list_t *new_node, *node;

    if (!head)
        return (NULL);

    node = *head;
    new_node = custom_malloc(sizeof(list_t));

    if (!new_node)
        return (NULL);

    custom_memset((void *)new_node, 0, sizeof(list_t));
    new_node->index = index;

    if (data)
    {
        new_node->data = custom_strdup(data);

        if (!new_node->data)
        {
            custom_free(new_node);
            return (NULL);
        }
    }

    if (node)
    {
        while (node->next)
            node = node->next;
        node->next = new_node;
    } else 
    {
        *head = new_node;
    }

    return (new_node);
}

/**
 * print_list_str - Print only the data element of a custom list.
 * @head: Pointer to the first node.
 *
 * Return: Size of the list.
 */
size_t print_list_str(const list_t *head)
{
    size_t size = 0;

    while (head) 
    {
        custom_puts(head->data ? head->data : "(nil)");
        custom_puts("\n");
        head = head->next;
        size++;
    }

    return (size);
}

/**
 * delete_node_at_index - Delete a node at a given index in the custom list.
 * @head: Address of the pointer to the first node.
 * @index: Index of the node to delete.
 *
 * Return: 1 on success, 0 on failure.
 */
int delete_node_at_index(list_t **head, unsigned int index)
{
    list_t *node, *prev_node;
    unsigned int i = 0;

    if (!head || !*head)
        return (0);

    if (!index)
    {
        node = *head;
        *head = (*head)->next;
        free(node->data);
        free(node);
        return (1);
    }

    node = *head;

    while (node)
    {
        if (i == index)
        {
            prev_node->next = node->next;
            free(node->data);
            free(node);
            return (1);
        }

        i++;
        prev_node = node;
        node = node->next;
    }

    return (0);
}

/**
 * free_list - Free all nodes of the custom list.
 * @head_ptr: Address of the pointer to the head node.
 *
 * Return: void.
 */
void free_list(list_t **head_ptr)
{
    list_t *node, *next_node, *head;

    if (!head_ptr || !*head_ptr)
        return;

    head = *head_ptr;
    node = head;

    while (node)
    {
        next_node = node->next;
        free(node->data);
        free(node);
        node = next_node;
    }

    *head_ptr = NULL;
}
