/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 15:06:02 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:46:00 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file expansions2.c
 * @brief Helper functions for navigating the expansion scanning
 */

/**
 * @brief Sets malloc fail error flag in case of failure
 * @param data The main data structure of the program
 * @param value The location in the memory that was supposed to be allocated
 * @retval 1 (mem error set)
 * @retval 0 (no mem error)
 */
int	set_mem_error(t_data *data, char *value)
{
	if (!value)
	{
		data->mem_error = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Jump to next node
 * @param current Current position in lexed linked list
 * @param prev Previous node in the lexed linked list
 */
void	advance_node(t_lexer **current, t_lexer **prev)
{
	if (current && *current)
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

/**
 * @brief Checks if ' quote
 * @param current Current position in lexed linked list
 * @param prev Previous node in the lexed linked list
 */
int	is_single_quote(t_lexer **current, t_lexer **prev)
{
	if ((*current)->value[0] == '\'')
	{
		advance_node(current, prev);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if previous node is redirection node
 * @param current Current position in lexed linked list
 * @param prev Previous node in the lexed linked list
 */
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
