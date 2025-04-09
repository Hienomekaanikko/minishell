/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:14 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/09 15:50:21 by msuokas          ###   ########.fr       */
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

//token tyypit
typedef enum e_token
{
	EMPTY,
	CMD,
	ARG,
	PIPE,
	RE_IN,
	RE_OUT
	//näitä tarvii varmaa lisää mut täs nää perushommat
} t_token;

//linked list structure
typedef struct s_lexer
{
	char			*value;
	t_token			type;
	struct s_lexer	*next;
}	t_lexer;

typedef struct	s_ast
{
	char			*value;
	char			*arguments;
	t_token			type;
	struct s_ast	*left;
	struct s_ast	*right;
} t_ast;

//structure for the main data stuff
typedef struct s_data
{
	char	*str;
	char	**new_str_arr;
	int		amount_of_pipes;
	t_lexer	**lexed_list;
	t_ast	*ast;
}	t_data;

int		ft_make_list(t_lexer **a, char **content);
void	make_ast(t_data *data);

#endif
