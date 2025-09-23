/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:34:40 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:03:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file ast_utils2.c
 * @brief Helper functions (part 2) for AST building
 */

/**
 * @brief This is just to interate (the school has 25 line function lenght limit, which means there's some of these weird solutions)
 * @param i Current index
 * @param len Current length
 */
void	increment_pointers(int *i, int *len)
{
	(*i)++;
	(*len)++;
}

/**
 * @brief Counts new length of a value after quotes have been removed (to allocate correct amount of memory)
 * @param value Value that is being handled
 * @retval New length
 */
int	count_new_len(char *value)
{
	int		i;
	int		len;
	char	quote;

	i = 0;
	len = 0;
	quote = 0;
	while (value[i])
	{
		if (!quote && (value[i] == '\'' || value[i] == '"'))
			quote = value[i++];
		else if (quote)
		{
			if (value[i] == quote)
			{
				quote = 0;
				i++;
			}
			else
				increment_pointers(&i, &len);
		}
		else
			increment_pointers(&i, &len);
	}
	return (len);
}

/**
 * @brief Allocates and creates new AST-node with NULL initialized data (new_node->access is 1, meaning TRUE, unless some permission issues are found later in processing)
 * @param value Value that is being dragged into the node's value from the lexed linked list
 * @param type Type of the value that has been set into the lexed linked list (RE_IN, RE_OUT, APPEND_OUT, HERE_DOC, PIPE, CMD, ARG)
 * @retval Newly created t_ast node.
 */
t_ast	*create_node(char *value, t_token type)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	if (has_quotes(value))
	{
		new_node->value = remove_quotes(value);
		if (node_error(new_node))
			return (NULL);
	}
	else
	{
		new_node->value = ft_strdup(value);
		if (node_error(new_node))
			return (NULL);
	}
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->access = 1;
	new_node->args = NULL;
	new_node->path = NULL;
	new_node->file = NULL;
	return (new_node);
}

/**
 * @brief Counts the size of the lexed linked list
 * @param current Current node inside the lexed linked list
 * @retval Size of the lexed linked list
 */
int	count_size(t_lexer *current)
{
	t_lexer	*temp;
	int		i;

	i = 0;
	temp = current;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

/**
 * @brief Checks if a value has quotes
 * @param value Value that is being checked
 * @retval 1 (has quotes)
 * @retval 0 (does not have quotes)
 */
int	has_quotes(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			return (1);
		i++;
	}
	return (0);
}
