#include "my_shell.h"

/**
 * custom_exit - Exit the custom shell
 * @info: Structure containing potential arguments and shell information.
 *
 * Return: Exits the shell given an exit status(0 if info->argv[0] != "exit").
 */
int custom_exit(custom_shell_info_t *info)
int custom_exit(custom_shell_info_t *info)
{
	int exit_status;

	if (info->argv[1])
	{
	exit_status = custom_atoi(info->argv[1]);
	if (exit_status == -1)
	{
		info->status = 2;
		print_error(info, "Illegal number: ");
		_eputs(info->argv[1]);
		_eputchar('\n');
		return (1);
	}
	info->err_num = exit_status;
	return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * Handle_cd - Handles "cd" operation and update environment variables.
 * @info: Structure containing potential arguments and shell information.
 * @buffer: A character buffer for storing the current directory.
 * @new_dir: The new directory to change to.
 *
 * Return: 0 if successful, 1 if there's an error.
 */
static int handle_cd(custom_shell_info_t *info, char *buffer, char *new_dir)
{
	int chdir_ret = chdir(new_dir);

	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(info->argv[1]);
		_eputchar('\n');
		return (1);
	}

	_setenv(info, "OLDPWD", _getenv(info, "PWD="));
	_setenv(info, "PWD", getcwd(buffer, 1024));

	return (0);
}

/**
 * custom_cd - Change the current directory of the custom shell process
 * @info: Structure containing potential arguments and shell information.
 *
 * Return: Always 0
 */
int custom_cd(custom_shell_info_t *info)
{
	char *current_dir, *new_dir, buffer[1024];

	current_dir = getcwd(buffer, 1024);
	if (!current_dir)
	{
		_puts("TODO: >>getcwd failure emsg here<<\n");
	}

	if (!info->argv[1])
	{
		new_dir = _getenv(info, "HOME=");
	if (!new_dir)
	{
		new_dir = _getenv(info, "PWD=");
	}
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
	if (!_getenv(info, "OLDPWD="))
	{
		_puts(current_dir);
		_putchar('\n');
		return (1);
	}
	new_dir = _getenv(info, "OLDPWD=");
	}
	else
	{
	new_dir = info->argv[1];
	}

	return (handle_cd(info, buffer, new_dir));
}

/**
 * custom_help - Display help information for the custom shell
 * @info: Structure containing potential arguments and shell information.
 *
 *Return: Always 0
 */
int custom_help(custom_shell_info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented\n");

	if (0)
	{
		_puts(*arg_array); /* Temp att_unused workaround */
	}

	return (0);
