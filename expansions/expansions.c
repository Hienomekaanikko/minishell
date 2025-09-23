/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:45:58 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file expansions.c
 * @brief Functions to look for expansions
 */

/**
 * @brief Replaces a $something value inside the lexed linked list to the expanded value
 * @param data The main data structure of the program
 * @param current Current position in the lexed linked list
 * @param exp The expanded value
 */
void	replace_value(t_data *data, t_lexer **current, char *exp)
{
	data->tools->result = NULL;
	free((*current)->value);
	(*current)->value = exp;
}

/**
 * @brief Handles the situation when no key was found
 * @param data The main data structure of the program
 * @param current Current position in the lexed linked list
 * @param prev Previous node in the lexed linked list
 * @retval 1 (success)
 * @retval 0 (previous is a redirection operator thus no sense to do anything)
 */
int	handle_non_found(t_data *data, t_lexer **current, t_lexer **prev)
{
	if (prev_is_redir(current, prev))
		return (0);
	else
		*current = remove_key_not_found(data, *current, *prev);
	return (1);
}

/**
 * @brief The beginning for the expansion checking
 * @param data The main data structure of the program
 */
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
				continue ;
		}
		advance_node(&current, &prev);
	}
}
