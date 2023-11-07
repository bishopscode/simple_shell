#include "my_shell.h"

/**
 * custom_hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_hsh(custom_shell_info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);
		if (custom_interactive(info))
			custom_puts("$ ");
		custom_eputchar(BUF_FLUSH);
		r = custom_get_input(info);
		if (r != -1)
		{
			custom_set_info(info, av);
			builtin_ret = custom_find_builtin(info);
			if (builtin_ret == -1)
				custom_find_cmd(info);
		}
		else if (custom_interactive(info))
			custom_putchar('\n');
		custom_free_info(info, 0);
	}
	custom_write_history(info);
	custom_free_info(info, 1);
	if (!custom_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * custom_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int custom_find_builtin(custom_shell_info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", custom_myexit},
		{"env", custom_myenv},
		{"help", custom_myhelp},
		{"history", custom_myhistory},
		{"setenv", custom_mysetenv},
		{"unsetenv", custom_myunsetenv},
		{"cd", custom_mycd},
		{"alias", custom_myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (custom_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * custom_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void custom_find_cmd(custom_shell_info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!custom_is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = custom_find_path(info, custom_getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		custom_fork_cmd(info);
	}
	else
	{
		if ((custom_interactive(info) || custom_getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && custom_is_cmd(info, info->argv[0]))
			custom_fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			custom_print_error(info, "not found\n");
		}
	}
}

/**
 * custom_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void custom_fork_cmd(custom_shell_info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, custom_get_environ(info)) == -1)
		{
			custom_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				custom_print_error(info, "Permission denied\n");
		}
	}
}
