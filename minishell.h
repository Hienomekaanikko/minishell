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
	char			**env;
	char			*infile;
	char			*outfile;
	struct s_ast	*left;
	struct s_ast	*right;
} t_ast;

typedef struct s_exec_status
{
	int			exit_code;		// return status, 0 = success, non-zero = failure
	int			signal;			//0 for built-ins
	char		*error_msg;		//Error message or NULL = success
	pid_t		pid;			 //0 for built-ins
	void		(*cleanup)(void*); // Cleanup function? need to research more
} t_exec_status;

//structure for the main data stuff
typedef struct s_data
{
	t_lexer	**lexed_list;
	t_ast	*root;
	char	**temp_array;
	char	*input;
}	t_data;

int		ft_make_list(t_data *data);
void	make_tree(t_data *data);
void	visualize_tree_TEST(t_data *data);
void    execute_command(t_ast *node, char **env, t_exec_status *exec_status); //MB, ajetaan niitä commandeja
//builtins
int	builtin_echo(char **args);
int	builtin_cd();
int	builtin_pwd();
int	builtin_export();
int	builtin_unset();
int	builtin_env();

#endif
