#ifndef my_shell_h_
#define my_shell_h_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR	1
#define CMD_AND	2
#define CMD_CHAIN	3

/* for custom_convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".custom_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field of the node
 * @str: a string
 * @next: pointing to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains pseudo-arg to pass into a function,
 * allows for uniform prototype for function ptr struct
 * @arg: a string generated from getline containing arguments
 * @argv: an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on, count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @history: the history node
 * @alias: the alias node
 * @environ: custom modified copy of environ from LL env
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, for memory management
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* ptr to cmd ; chain buffer, for mem management */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} custom_shell_info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 * struct builtin - containing builtin string and related function
 * @type: the custom builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(custom_shell_info_t *);
} builtin_table;

/* toem_custom_shell_loop.c */
int custom_hsh(custom_shell_info_t *, char **);
int custom_find_builtin(custom_shell_info_t *);
void custom_find_cmd(custom_shell_info_t *);
void custom_fork_cmd(custom_shell_info_t *);

/* toem_custom_parser.c */
int custom_is_cmd(custom_shell_info_t *, char *);
char *custom_dup_chars(char *, int, int);
char *custom_find_path(custom_shell_info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_errors_handling.c */
void custom_eputs(char *);
int custom_eputchar(char);
int custom_putfd(char c, int fd);
int custom_putsfd(char *str, int fd);

/* toem_string_man.c */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *custom_starts_with(const char *, const char *);
char *custom_strcat(char *, char *);

/* toem_string_man1.c */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* toem_string_manipulations.c */
char *custom_strncpy(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* toem_tokenize.c */
char **custom_strtow(char *, char *);
char **custom_strtow2(char *, char);

/* toem_realm.c */
char *custom_memset(char *, char, unsigned int);
void custom_ffree(char **);
void *custom_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int bfree(void **);

/* toem_custom_utils.c */
int custom_interactive(custom_shell_info_t *);
int custom_is_delim(char, char *);
int custom_isalpha(int);
int custom_atoi(char *);

/* toem_error_handling2.c */
int custom_erratoi(char *);
void custom_print_error(custom_shell_info_t *, char *);
int custom_print_d(int, int);
char *custom_convert_number(long int, int, int);
void custom_remove_comments(char *);

/* toem_builtin.c */
int custom_myexit(custom_shell_info_t *);
int custom_mycd(custom_shell_info_t *);
int custom_myhelp(custom_shell_info_t *);

/* toem_builtin2.c */
int custom_myhistory(custom_shell_info_t *);
int custom_myalias(custom_shell_info_t *);

/*toem_input.c */
ssize_t custom_get_input(custom_shell_info_t *);
int custom_getline(custom_shell_info_t *, char **, size_t *);
void custom_sigintHandler(int);

/* toem_info.c */
void custom_clear_info(custom_shell_info_t *);
void custom_set_info(custom_shell_info_t *, char **);
void custom_free_info(custom_shell_info_t *, int);

/* toem_environment_manager.c */
char *custom_getenv(custom_shell_info_t *, const char *);
int custom_myenv(custom_shell_info_t *);
int custom_mysetenv(custom_shell_info_t *);
int custom_myunsetenv(custom_shell_info_t *);
int custom_populate_env_list(custom_shell_info_t *);

/* toem_environment.c */
char **custom_get_environ(custom_shell_info_t *);
int custom_unsetenv(custom_shell_info_t *, char *);
int custom_setenv(custom_shell_info_t *, char *, char *);

/* toem_history.c */
char *custom_get_history_file(custom_shell_info_t *info);
int custom_write_history(custom_shell_info_t *info);
int custom_read_history(custom_shell_info_t *info);
int custom_build_history_list(custom_shell_info_t *info, char *buf, int linecount);
int custom_renumber_history(custom_shell_info_t *info);

/* toem_custom_list.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* toem_custom_lists2.c */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* toem_chaining.c */
int custom_is_chain(custom_shell_info_t *, char *, size_t *);
void custom_check_chain(custom_shell_info_t *, char *, size_t *, size_t, size_t);
int custom_replace_alias(custom_shell_info_t *);
int custom_replace_vars(custom_shell_info_t *);
int custom_replace_string(char **, char *);

#endif
