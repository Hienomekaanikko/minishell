/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:34:40 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/14 15:26:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_new_len(char *value)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			i++;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

//creates a node with places for childs and args etc data
t_ast	*create_node(char *value, t_token type)
{
	t_ast *new_node = (t_ast*)malloc(sizeof(t_ast));
	if (has_quotes(value))
		new_node->cmd = remove_quotes(value);
	else
		new_node->cmd = ft_strdup(value);
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->access = 1;
	new_node->args = NULL;
	new_node->file = NULL; //MB ATTENTION: infile outfile -> file
	return (new_node);
}

//counts the amount of arguments to allocate right amount of memory for the array of argument strings
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
