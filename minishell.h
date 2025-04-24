/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:14 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/11 15:30:13 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

#define SECURE_PATH "//bin:/usr/bin:/usr/local/bin"  //MB. Execve checks this first

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
	char			*infile;
	char			*outfile;
	struct s_ast	*left;
	struct s_ast	*right;
} t_ast;

//structure for the main data stuff
typedef struct s_data
{
	t_lexer	**lexed_list;
	t_ast	*root;
	char	**temp_array;
	char	*input;
}	t_data;

//structure for the execution status
typedef struct s_exec_status
{
	int			exit_code;
	int			signal;
	char		*error_msg;
	pid_t		pid;
} t_exec_status;

//arena
typedef struct s_arena
{
	char		*memory;
	char		**ptrs;
	size_t		mem_size;
	size_t		mem_used;
	size_t		ptrs_in_use;
	size_t		ptr_capacity;
}	t_arena;

int		ft_make_list(t_data *data);
void	make_tree(t_data *data);
void	visualize_tree_TEST(t_data *data);
//void	debug_print(char *msg);
//execution
void    execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status);
void	exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *exec_status);
//arena
t_arena	*arena_init(size_t arena_size, size_t initial_ptrs);
void	arena_free(t_arena *arena);
char	*arena_add(t_arena *arena, char *str);
void	arena_clear(t_arena *arena);
t_arena	*init_env_arena(char **envp);
//built-ins
int		builtin_echo(char **args, t_exec_status *status);
int		builtin_cd(char **args);
int		builtin_pwd();
int		builtin_export();
int		builtin_unset();
int		builtin_env(t_arena *env_arena);
//envp
char	*arena_getenv(t_arena *env_arena, char *key);

#endif
