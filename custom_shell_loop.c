#include "my_shell.h"

/**
 * custom_shell_loop - main shell loop
 * @info: the parameter & return custom_info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int custom_shell_loop(custom_info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		custom_clear_info(info);
		if (custom_interactive(info))
			custom_puts("$ ");
		custom_eputchar(CUSTOM_BUF_FLUSH);
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
 * custom_find_builtin - finds a custom builtin command
 * @info: the parameter & return custom_info struct
 *
 * Return: -1 if custom builtin not found,
 *         0 if custom builtin executed successfully,
 *         1 if custom builtin found but not successful,
 *         -2 if custom builtin signals exit()
 */
int custom_find_builtin(custom_info_t *info)
{
	int i, built_in_ret = -1;
	custom_builtin_table custom_builtintbl[] = {
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

	for (i = 0; custom_builtintbl[i].type; i++)
		if (custom_strcmp(info->argv[0], custom_builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = custom_builtintbl[i].func(info);
			break;
	}
	return (built_in_ret);
}
