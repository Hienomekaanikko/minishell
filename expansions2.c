/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:06:02 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 16:21:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_mem_error(t_data *data, char *value)
{
	if (!value)
	{
		data->mem_error = 1;
		return (1);
	}
	return (0);
}

void	advance_node(t_lexer **current, t_lexer **prev)
{
	if (current && *current)
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

int	is_single_quote(t_lexer **current, t_lexer **prev)
{
	if ((*current)->value[0] == '\'')
	{
		advance_node(current, prev);
		return (1);
	}
	return (0);
}

int	prev_is_redir(t_lexer **curr, t_lexer **prev)
{
	if (prev && *prev && ((*prev)->type == RE_IN
			|| (*prev)->type == RE_OUT || (*prev)->type == APPEND_OUT
			|| (*prev)->type == HERE_DOC))
	{
		advance_node(curr, prev);
		return (1);
	}
	return (0);
}
