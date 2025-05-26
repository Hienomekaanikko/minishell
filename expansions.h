/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:20:01 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 12:01:35 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
#define EXPANSIONS_H

# include "minishell.h"

typedef struct s_keys
{
	char			*key;
	struct s_keys	*next;
}	t_keys;

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
	int		key_len;
	size_t	i;
	size_t	start;
	char	*extracted_key;
	char	*fetched_value;
	char	*leftovers;
	char	*new_value;
	char	*expanded_value;
}	t_exp_tools;


#endif
