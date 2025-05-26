/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:48:40 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 14:22:40 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_BUILDER_H
# define AST_BUILDER_H
# include "minishell.h"

typedef struct s_ast_utils
{
	int		i;
	int		j;
	int		len;
	int		quote;
	int		argument_amount;
	char	*cleaned_value;
}	t_ast_utils;

#endif
