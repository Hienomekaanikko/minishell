/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:31:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:05:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	node_fail(t_data *data, t_ast *node)
{
	if (!node)
	{
		data->mem_error = 1;
		return (1);
	}
	return (0);
}

void	set_followup_redir(t_data *data, t_lexer *curr, t_ast *new)
{
	new = create_node(curr->value, curr->type);
	if (node_fail(data, new))
		return ;
	if (new->type == HERE_DOC)
	{
		if (write_heredoc(data, &curr->next->value, &new->file) == -1)
		{
			free_ast(new);
			return ;
		}
	}
	new->left = data->root;
	curr = curr->next;
	if (curr && curr->type == ARG)
	{
		add_right_child(&new->right, curr);
		if (is_child_failure(data, new->right))
			return ;
	}
	if (curr && !data->mem_error
		&& ((perms(data, new->right->args[0], new->type) == -1)
			|| data->redir_err == 1))
		set_access_err(data, new);
	if (curr && !data->mem_error)
		data->root = new;
}

void	set_redir_root(t_data *data, t_lexer *prev_cmd, t_lexer *curr)
{
	data->root = create_node(curr->value, curr->type);
	if (node_fail(data, data->root))
		return ;
	if (data->root->type == HERE_DOC)
	{
		if (write_heredoc(data, &curr->next->value, &data->root->file) == -1)
			return ;
	}
	add_left_child(&data->root->left, prev_cmd, curr);
	if (prev_cmd && is_child_failure(data, data->root->left))
		return ;
	curr = curr->next;
	if (curr && !data->mem_error && curr->type == ARG)
	{
		add_right_child(&data->root->right, curr);
		if (is_child_failure(data, data->root->right))
			return ;
	}
	if (curr && !data->mem_error
		&& ((perms(data, data->root->right->args[0], data->root->type) == -1)
			|| data->redir_err == 1))
		set_access_err(data, data->root);
}

void	set_followup_pipe(t_data *data, t_lexer *curr, t_ast *new)
{
	new = create_node(curr->value, curr->type);
	if (!new)
	{
		data->mem_error = 1;
		return ;
	}
	new->left = data->root;
	curr = curr->next;
	if (curr && curr->type == CMD)
	{
		add_right_child(&new->right, curr);
		if (is_child_failure(data, new->right))
			return ;
	}
	if (!data->mem_error)
		data->root = new;
}

void	set_first_pipe(t_data *data, t_lexer *curr, t_lexer *prev_cmd)
{
	data->root = create_node(curr->value, curr->type);
	if (!data->root)
	{
		data->mem_error = 1;
		return ;
	}
	curr = curr->next;
	if (curr && prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd, curr);
		if (is_child_failure(data, data->root->left))
			return ;
		prev_cmd = NULL;
	}
	if (curr && curr->type == CMD)
	{
		add_right_child(&data->root->right, curr);
		if (is_child_failure(data, data->root->right))
			return ;
	}
}
