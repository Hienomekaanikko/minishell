/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:20:01 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/10 13:02:54 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
# define EXPANSIONS_H

# include "minishell.h"

typedef struct s_var
{
	char			*key;
	char			*value;
	struct s_var	*next;
}	t_var;

typedef struct s_exp_data
{
	t_var	*var_list;
}	t_exp_data;

typedef struct s_exp_tools
{
	char	*result;
	char	*var;
	char	*val;
	char	*temp;
}	t_exp_tools;

#endif
