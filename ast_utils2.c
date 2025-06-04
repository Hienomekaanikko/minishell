/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:34:40 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 14:39:44 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increment_pointers(int *i, int *len)
{
	(*i)++;
	(*len)++;
}

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

t_ast	*create_node(char *value, t_token type)
{
	t_ast	*new_node;

	new_node = (t_ast *)malloc(sizeof(t_ast));
	if (!new_node)
		return (NULL);
	if (has_quotes(value))
	{
		new_node->cmd = remove_quotes(value);
		if (node_error(new_node))
			return (NULL);
	}
	else
	{
		new_node->cmd = ft_strdup(value);
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
