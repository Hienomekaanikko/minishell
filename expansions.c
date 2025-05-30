/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/29 10:25:16 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_value(t_data *data, t_lexer **current, char *exp)
{
	data->tools->result = NULL;
	free((*current)->value);
	(*current)->value = exp;
}

int	handle_non_found(t_data *data, t_lexer **current, t_lexer **prev)
{
	if (prev_is_redir(current, prev))
		return (0);
	else
		*current = remove_key_not_found(data, *current, *prev);
	return (1);
}

void	check_for_expansions(t_data *data)
{
	t_lexer	*current;
	t_lexer	*prev;
	char	*expanded_value;

	current = *data->lexed_list;
	prev = NULL;
	while (current)
	{
		expanded_value = NULL;
		if (is_single_quote(&current, &prev))
			continue ;
		if (ft_strchr(current->value, '$'))
		{
			expanded_value = expand(data, data->tools, current->value);
			if (data->mem_error)
				return ;
			if (expanded_value)
				replace_value(data, &current, expanded_value);
			else if (!expanded_value && handle_non_found(data, &current, &prev))
				continue;
		}
		advance_node(&current, &prev);
	}
	free_exp_tools(data);
}
