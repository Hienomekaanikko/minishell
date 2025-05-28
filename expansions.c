/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 18:40:55 by msuokas          ###   ########.fr       */
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

void	check_for_expansions(t_data *data)
{
	t_lexer	*current;
	t_lexer	*prev;
	char	*expanded_value;

	current = *data->lexed_list;
	prev = NULL;
	while (current)
	{
		if (is_single_quote(&current, &prev))
			continue ;
		if (ft_strchr(current->value, '$'))
		{
			expanded_value = expand(data, data->tools, current->value);
			if (data->mem_error)
				return ;
			if (expanded_value)
			{
				data->tools->result = NULL;
				free(current->value);
				current->value = expanded_value;
			}
			else
				current = remove_key_not_found(data, current, prev);
			advance_node(&current, &prev);
		}
		else
			advance_node(&current, &prev);
	}
	free_exp_tools(data);
}
