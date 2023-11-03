#include "my_shell.h"

/**
 * init_custom_info - Initialize custom_shell_info_t struct.
 * @info: Struct to initialize.
 */
void init_custom_info(custom_shell_info_t *info)
{
    info->custom_arg = NULL;
    info->custom_args = NULL;
    info->custom_path = NULL;
    info->custom_argc = 0;
}

/**
 * fill_custom_info - Fill custom_shell_info_t struct with custom arguments.
 * @info: Struct to fill.
 * @custom_argv: Custom argument vector.
 */
void fill_custom_info(custom_shell_info_t *info, char **custom_argv)
{
    int i = 0;

    info->custom_name = custom_argv[0];

    if (info->custom_arg) 
    {
        info->custom_args = custom_strtow(info->custom_arg, " \t");
        if (!info->custom_args) 
        {
            info->custom_args = custom_malloc(sizeof(char *) * 2);
            if (info->custom_args)
            {
                info->custom_args[0] = custom_strdup(info->custom_arg);
                info->custom_args[1] = NULL;
            }
        }

        for (i = 0; info->custom_args && info->custom_args[i]; i++)
            ;
        info->custom_argc = i;

        custom_replace_alias(info);
        custom_replace_vars(info);
    }
}

/**
 * cleanup_custom_info - Free memory associated with custom_shell_info_t struct.
 * @info: Struct to clean up.
 * @free_all: true if all fields should be freed.
 */
void cleanup_custom_info(custom_shell_info_t *info, int free_all)
{
    custom_ffree(info->custom_args);
    info->custom_args = NULL;
    info->custom_path = NULL;

    if (free_all)
    {
        if (!info->custom_cmd_buf)
            custom_free(info->custom_arg);
        if (info->custom_env)
            custom_free_list(&(info->custom_env));
        if (info->custom_history)
            custom_free_list(&(info->custom_history));
        if (info->custom_alias)
            custom_free_list(&(info->custom_alias));
        custom_ffree(info->custom_environ);
        info->custom_environ = NULL;
        custom_bfree((void **)info->custom_cmd_buf);
        if (info->custom_readfd > 2)
            custom_close(info->custom_readfd);
        custom_putchar(CUSTOM_BUF_FLUSH);
    }
}
