#include "my_shell.h"

/**
 * custom_myexit - Exit the custom shell
 * @info: Structure containing potential arguments and shell information.
 *
 * Return: Exits the shell given an exit status(0 if info->argv[0] != "exit").
 */
int custom_myexit(custom_shell_info_t *info)
{
	int exitchecker;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitchecker = custom_erratoi(info->argv[1]);
		if (exitchecker == -1)
		{
			info->status = 2;
			custom_print_error(info, "Illegal number: ");
			custom_eputs(info->argv[1]);
			custom_eputchar('\n');
			return (1);
		}
		info->err_num = custom_erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * custom_mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int custom_mycd(custom_shell_info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		custom_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = custom_getenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = custom_getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (custom_strcmp(info->argv[1], "-") == 0)
	{
		if (!custom_getenv(info, "OLDPWD="))
		{
			custom_puts(s);
			custom_putchar('\n');
			return (1);
		}
		custom_puts(custom_getenv(info, "OLDPWD=")), custom_putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = custom_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		custom_print_error(info, "can't cd to ");
		custom_eputs(info->argv[1]), custom_eputchar('\n');
	}
	else
	{
		custom_setenv(info, "OLDPWD", custom_getenv(info, "PWD="));
		custom_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}
/**
 * custom_help - Display help information for the custom shell
 * @info: Structure containing potential arguments and shell information.
 *
 *Return: Always 0
 */
int custom_myhelp(custom_shell_info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	custom_puts("help call works. Function not yet implemented\n");

	if (0)
	{
		custom_puts(*arg_array); /* Temp att_unused workaround */
	}

	return (0);
