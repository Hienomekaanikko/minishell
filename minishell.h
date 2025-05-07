/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:14 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/25 17:59:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include "expansions.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <errno.h>
#define SECURE_PATH "//bin:/usr/bin:/usr/local/bin"  //MB. Execve checks this first

int g_exit_value;

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
	APPEND_OUT
	//näitä tarvii varmaa lisää mut täs nää perushommat
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

//structure for the main data stuff
typedef struct s_data
{
	t_exp_data	*exp;
	t_lexer		**lexed_list;
	t_ast		*root;
	char		**temp_array;
	char		*input;
	t_arena		*env_arena;
	int			syntax_err;
}	t_data;

//structure for the execution status
typedef struct	s_exec_status
{
	int			exit_code;
	int			signal;
	char		*error_msg;
	pid_t		pid;
}	t_exec_status;

//lexer stuff
int			ft_make_list(t_data *data);
void		make_tree(t_data *data);
int			ft_add_node(t_lexer **list, char *input_list);
void		add_starting_token(t_lexer *curr);
void		add_token(t_lexer *curr, t_lexer *prev);
int			ft_lexer(t_data *data); //env-arena added

//mikko memory stuff
void		destroy_memory(t_data *data);
void		free_lexed_list(t_lexer *start);
void		free_ast(t_ast *root);

//ast tree stuff (added 22.4.)
void		add_arguments(t_ast *curr_node, t_lexer *current);
void		add_right_child(t_ast **position, t_lexer *current);
void		add_left_child(t_ast **position, t_lexer *prev_cmd);
void		set_complex_tree(t_data *data);
char		*remove_quotes(char *value);
int			count_new_len(char *value);
int			count_size(t_lexer *current);
t_ast		*create_node(char *value, t_token type);
int			has_quotes(char *value);

//var declaration stuff
int			is_var_declaration(char	*str);
void		add_var_declaration(t_data *data);
int			already_declared(t_var *start, char *key, char *value);

//expansion stuff
void		check_for_expansions(t_data *data);
int			count_dollars(t_lexer *curr);
char		*is_declared(t_data *data, char *extracted_key);
void		refresh_value(t_lexer *current, char *expanded_value, t_lexer *prev);
t_lexer		*remove_key_not_found(t_data *data, t_lexer *current, t_lexer *prev);

void		visualize_tree_TEST(t_data *data);
//execution
int    	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena);
char	*find_executable(t_ast *node, t_arena *env_arena);
int		exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena);
void	wait_process(pid_t pid, t_exec_status *exec_status);
int		exec_redir(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena);
int		exec_heredoc(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena);
//error
int		error_handler(t_exec_status *status, const char *msg, int exit_code);
void	handle_signal_error(t_exec_status *status, int signal);
//arena
t_arena	*arena_init(size_t arena_size, size_t initial_ptrs);
void	arena_free(t_arena *arena);
char	*arena_add(t_arena *arena, char *str);
void	arena_clear(t_arena *arena);
//built-ins
int		builtin_echo(char **args, t_exec_status *status);
int		builtin_cd(char **args, t_exec_status *status);
int		builtin_pwd(t_exec_status *status);
int		builtin_export(t_arena *env_arena, t_exec_status *status, char **args);
int		builtin_unset(t_arena *env_arena, t_exec_status *status, char **args);
int		builtin_env(t_arena *env_arena, t_exec_status *status);
int		builtin_exit(char **args, t_exec_status *status);
//envp
t_arena	*init_env_arena(char **envp);
char	*arena_getenv(t_arena *env_arena, char *key);
int		arena_set_env(t_arena *env_arena, char *key, char *value);
int		arena_unset_env(t_arena *env_arena, char *key);
//signals
void	setup_signals(void);
void	handle_sigint(int sig);
void	setup_child_signals(void);
void	handle_heredoc_signals(void);
//graphics
void	splash_screen(void);


#endif
