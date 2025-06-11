/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:53:13 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:47:02 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_basic_tree(t_data *data)
{
	t_lexer	*current;

	current = *data->lexed_list;
	if (current)
	{
		add_right_child(&data->root, current);
		if (!data->root)
		{
			data->mem_error = 1;
			return ;
		}
		if (!data->root->args)
		{
			data->mem_error = 1;
			return ;
		}
	}
}

int	tree_type(t_data *data)
{
	t_lexer	*temp;

	temp = *data->lexed_list;
	while (temp)
	{
		if (temp->type == PIPE || temp->type == RE_IN || temp->type == RE_OUT
			|| temp->type == HERE_DOC || temp->type == APPEND_OUT)
			return (2);
		temp = temp->next;
	}
	return (1);
}

void	make_tree(t_data *data)
{
	data->root = NULL;
	if (tree_type(data) == 1)
		set_basic_tree(data);
	else if (tree_type(data) == 2)
		set_complex_tree(data);
	if (data->mem_error || data->redir_err == 2)
	{
		if (data->root)
			free_ast(data->root);
		data->root = NULL;
	}
	if (*data->lexed_list)
		free_lexed_list(*data->lexed_list);
}
