#ifndef MY_SHELL_H
#define MY_SHELL_H

/**
 * struct custom_info_s - holds parameters and return data
 * @alias: the alias list
 * @argv: argument vector
 * @buf: buffer for getline
 * @cmd_buf_type: the type of command buffer
 * @env: the environment
 * @err_num: the error number
 * @history: command history
 * @line_count: the command line count
 * @linecount_flag: a flag for counting lines
 * @path: the command path
 * @status: the exit status
 */
typedef struct custom_info_s
{
	list_t *alias;
	char **argv;
	char *buf;
	int cmd_buf_type;
	list_t *env;
	int err_num;
	list_t *history;
	int line_count;
	int linecount_flag;
	char *path;
	int status;
} custom_info_t;

#endif /* My_SHELL_H */