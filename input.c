#include "my_shell.h"

/**
 * custom_input_buf - Buffers chained commands in custom shell.
 * @info: Custom shell information struct.
 * @buf: Address of buffer to store the input.
 * @len: Address of buffer length variable.
 *
 * Return: Bytes read.
 */
ssize_t custom_input_buf(custom_shell_info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buffer, &len_p, stdin);
#else
		r = custom_getline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			custom_remove_comments(*buf);
			custom_build_history_list(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}
/**
 * custom_get_input - Gets a line of input without the newline character.
 * @info: Custom shell information struct.
 *
 * Return: Bytes read.
 */
ssize_t custom_get_input(custom_shell_info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	custom_putchar(BUF_FLUSH);
	r = custom_input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		custom_check_chain(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (custom_is_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (custom_strlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (r); /* return length of buffer from _getline() */
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
 * @ptr: Address of a pointer to the buffer (preallocated or NULL).
 * @length: Size of the preallocated pointer buffer if not NULL.
 *
 * Return: Size.
 */
int custom_getline(custom_shell_info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = custom_strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = custom_realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		custom_strncat(new_p, buf + i, k - i);
	else
		custom_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}
/**
 * custom_sigintHandler - Blocks Ctrl-C signal.
 * @sig_num: Signal number.
 *
 * Return: void.
 */
void custom_sigintHandler(__attribute__((unused)) int sig_num)
{
    custom_puts("\n");
    custom_puts("$ ");
    custom_putchar(CUSTOM_BUF_FLUSH);
}
