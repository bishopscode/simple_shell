#include "my_shell.h"

/**
 * custom_clear_info - Initialize custom_shell_info_t struct.
 * @info: Struct to initialize.
 */
void custom_clear_info(custom_shell_info_t *info)
{
    info->arg = NULL;
    info->args = NULL;
    info->path = NULL;
    info->argc = 0;
}

/**
 * custom_set_info - Setting custom_shell_info_t struct with custom arguments.
 * @info: Struct to fill.
 * @av: Custom argument vector.
 */
void custom_set_info(custom_shell_info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = custom_strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = custom_strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * custom_free_info - Free memory associated with custom_shell_info_t struct.
 * @info: Struct to clean up.
 * @all: true if all fields should be freed.
 */
void custom_free_info(custom_shell_info_t *info, int all)
{
    ffree(info->argv);
    info->argv = NULL;
    info->path = NULL;

    if (all)
    {
        if (!info->cmd_buf)
            free(info->arg);
        if (info->env)
            free_list(&(info->env));
        if (info->history)
            free_list(&(info->history));
        if (info->alias)
            free_list(&(info->alias));
        custom_ffree(info->environ);
        info->environ = NULL;
        bfree((void **)info->cmd_buf);
        if (info->custom_readfd > 2)
            close(info->readfd);
        custom_putchar(BUF_FLUSH);
    }
}
