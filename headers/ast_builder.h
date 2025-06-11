/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:48:40 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 11:50:17 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_BUILDER_H
# define AST_BUILDER_H
# include "minishell.h"

typedef struct s_utils
{
	int		i;
	int		j;
	int		len;
	int		quote;
	int		argument_amount;
	char	*cleaned_value;
}	t_utils;

#endif
