/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:14 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/29 17:04:19 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include "expansions.h"
# include "parser.h"
# include "ast_builder.h"
# include "parser.h"
# include "ast_builder.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>

extern volatile sig_atomic_t g_interrupted;

typedef enum e_error
{
	NO,
	AMB,
	MALLOC,
	NOT_VALID,
	TOO_MANY,
	NOENV,
	ONLYNUM,
	STDOUT,
	STDIN,
	FAIL,
	NOPERM,
	NOFILE,
	ISDIR,
	NOCMD,
} t_error;

//token types
typedef enum e_token
{
	EMPTY,
	CMD,
	ARG,
	PIPE,
	RE_IN,
	RE_OUT,
	HERE_DOC,
	APPEND_OUT,
} t_token;

//for lexing part of parsing (mikon juttuja)
typedef struct s_lexer
{
	t_token			type;
	char			*value;
	struct s_lexer	*next;
}	t_lexer;

//ast nodes
typedef struct	s_ast
{
	t_token			type;
	int				access;
	char			*cmd;
	char			**args;
	char			*file;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

//arena
typedef struct	s_arena
{
	char		*memory;
	char		**ptrs;
	size_t		mem_size;
	size_t		mem_used;
	size_t		ptrs_in_use;
	size_t		ptr_capacity;
}	t_arena;

//structure for the execution status
typedef struct	s_exec_status
{
	int			exit_code;
	int			here_doc_flag;
	int			signal;
	int			temp_fd;
	int			saved_stdin;
	int			saved_stdout;
	int			infile;
	int			outfile;
	int			redir_fail;
	int			final_exit_code;
	char		*msg;
	char		*path;
	pid_t		pid;
}	t_exec_status;

//structure for the main data stuff
typedef struct s_data
{
	t_exp_data		*exp;
	t_exp_tools		*tools;
	t_exec_status	status;
	t_error			err;
	t_arena			*env_arena;
	t_lexer			**lexed_list;
	t_ast			*root;
	char			**temp_array;
	char			*input;
	int				redir_status;
	int				syntax_err;
	int				redir_err;
	int				mem_error;
}	t_data;

//init stuff
void		init_data(t_data *data);
int			init_base(t_data *data, int argc, char **argv, char **envp);

//lexer stuff
char		**parser(t_data *data, char const *s, char c);
int			ft_make_list(t_data *data);
void		make_tree(t_data *data);
int			ft_add_node(t_lexer **list, char *input_list);
void		add_starting_token(t_lexer *curr);
void		add_token(t_lexer *curr, t_lexer *prev);
int			ft_lexer(t_data *data);
int			add_substring(t_data *data, t_parser *p, char **array_of_strings, const char *s);

//parsing
void		add_operator(t_data *data, t_parser *pars, const char *s);
int			make_substring(t_parser *data, char const *s, char **array_of_strings);
char		**free_malloc(char **array_of_strings, int y);
void		skip_word(t_counter *data, const char *s);

//mikko memory stuff
void		destroy_memory(t_data *data);
void		free_lexed_list(t_lexer *start);
void		free_ast(t_ast *root);
void		close_all_fds(t_data *data);

//ast tree stuff (added 22.4.)
void		add_arguments(t_utils *ast, t_ast *curr_node, t_lexer *current, t_token type);
void		add_right_child(t_ast **position, t_lexer *current, t_token type);
void		add_left_child(t_ast **position, t_lexer *prev_cmd, t_token type);
void		set_complex_tree(t_data *data);
char		*remove_quotes(char *value);
int			count_new_len(char *value);
int			count_size(t_lexer *current);
t_ast		*create_node(char *value, t_token type);
int			has_quotes(char *value);
int			write_heredoc(t_data *data, char *delimiter, char **out_path);
int			perms(t_data *data, char *path, int type);
void		ast_error_check(t_data *data);
void		set_access_err(t_data *data, t_ast *new);
void		set_followup_redir(t_data *data, t_lexer *curr, t_ast *new);
void		set_redir_root(t_data *data, t_lexer *prev_cmd, t_lexer *curr);
void		set_followup_pipe(t_data *data, t_lexer *curr, t_ast *new);
void		set_first_pipe(t_data *data, t_lexer *curr, t_lexer *prev_cmd);

//var declaration stuff
int			is_var_declaration(char	*str);
void		add_var_declaration(t_data *data);
int			already_declared(t_var *start, char *key, char *value);

//expansion stuff
char		*expand(t_data *data, t_exp_tools *tools, char *value);
int			set_mem_error(t_data *data, char *value);
void		check_for_expansions(t_data *data);
int			count_dollars(t_lexer *curr);
char		*is_declared(t_data *data, char *extracted_key);
t_lexer		*remove_key_not_found(t_data *data, t_lexer *current, t_lexer *prev);
void		unset_local(t_var **head, char *key);
int			after_dollar(t_data *data, t_exp_tools *tools, char *value);
int			dollar_literal(t_exp_tools *tools, int *i);
int			before_dollar(t_data *data, t_exp_tools *tools, char *value, int i);
int			dollar(t_data *data, t_exp_tools *tools, char *value, int *i);
int			variable(t_data *data, t_exp_tools *tools, char *value, int *i);
int			exit_status(t_data *data, t_exp_tools *tools, int *i);
void		free_exp_tools(t_data *data);

//execution
int			execute_command(t_ast *node, t_data *data);
char		*find_executable(t_ast *node, t_data *data);
int			exec_pipe(t_ast *node, t_data *data);
void		wait_process(pid_t pid, t_exec_status *exec_status);
void		wait_right_process(pid_t pid, t_exec_status *exec_status);
int			exec_redir(t_ast *node, t_data *data);
void		check_path_permissions(char *path, t_exec_status *exec_status);
void		close_fds(t_exec_status *exec_status);
void		restore_orig_fd(t_data *data);
void		save_orig_fd(t_data *data);
int			cleanup_pipe(int pipe_fd[2], pid_t pidL, pid_t pidR);

//error
int			error_handler(t_exec_status *status, char *cmd, t_error err, int exit_code);
void		handle_signal_error(t_exec_status *status, int signal);
void		check_syntax_error(t_lexer *checker, char **msg, t_lexer **prev);

//arena
t_arena		*arena_init(size_t arena_size, size_t initial_ptrs);
void		arena_free(t_arena *arena);
char		*arena_add(t_arena *arena, char *add, t_exec_status *status);
t_arena		*arena_cleanup(char **ptrs, char *memory);
void		arena_free(t_arena *arena);
int			arena_has_key(t_arena *env_arena, char *key);

//built-ins
int			builtin_echo(char **args, t_exec_status *status);
int			builtin_cd(char **args, t_data *data);
int			builtin_pwd(t_data *data);
int			builtin_export(t_data *data, char **args);
int			builtin_unset(t_data *data, char **args);
int			builtin_env(t_data *data);
int			builtin_exit(t_ast *node, t_exec_status *status);
int			is_valid_env_name(const char *name);
int			set_export_value(t_data *data, const char *key, const char *arg);

//envp
t_arena		*init_env_arena(char **envp, t_data *data);
t_arena		*init_backup_env(t_data *data);
char		*arena_getenv(t_arena *env_arena, char *key);
int			arena_set_env(t_data *data, char *key, char *value);
int			arena_unset_env(t_arena *env_arena, char *key);
void		set_shell_level(t_data *data);

//signals
void		setup_shell_signals(void);
void		setup_child_signals(void);
void		setup_heredoc_signals(void);
//graphics
void		splash_screen(void);


#endif
