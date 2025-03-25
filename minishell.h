/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:14 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/25 14:25:00 by msuokas          ###   ########.fr       */
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

typedef enum e_token
{
	CMD,
	ARG,
	PIPE,
	RE_IN,
	RE_OUT
	//näitä tarvii varmaa lisää mut täs nää perus
} t_token;

typedef struct s_list
{
	char			*value;
	t_token			type;
	struct s_list	*next;
}	t_list;

typedef struct s_data
{
	char	*str;
	char	**new_str_arr;
	t_list	**linked_list;
}	t_data;

int	ft_make_list(t_list **a, char **content);

#endif
