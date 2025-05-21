/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:20:01 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/21 17:20:11 by msuokas          ###   ########.fr       */
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

#endif
