/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:31:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/29 10:25:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_followup_redir(t_data *data, t_lexer *curr, t_ast *new)
{
	new = create_node(curr->value, curr->type);
	if (!new)
	{
		data->mem_error = 1;
		return ;
	}
	if (new->type == HERE_DOC)
		write_heredoc(data, curr->next->value, &new->file);
	new->left = data->root;
	curr = curr->next;
	if (curr && curr->type == ARG)
	{
		add_right_child(&new->right, curr, new->type);
		if (!new->right)
		{
			data->mem_error = 1;
			return ;
		}
		if (!new->right->args)
		{
			data->mem_error = 1;
			return ;
		}
	}
	if (curr && !data->mem_error
		&& ((perms(data, new->right->args[0], new->type) == -1)
			|| data->redir_err == 1))
		set_access_err(data, new);
	if (curr && !data->mem_error)
		data->root = new;
}

//sets redirection as root if no previous roots
void	set_redir_root(t_data *data, t_lexer *prev_cmd, t_lexer *curr)
{
	data->root = create_node(curr->value, curr->type);
	if (!data->root)
	{
		data->mem_error = 1;
		return ;
	}
	if (data->root->type == HERE_DOC)
		write_heredoc(data, curr->next->value, &data->root->file);
	curr = curr->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd, prev_cmd->type);
		if (!data->root->left->args)
			data->mem_error = 1;
		prev_cmd = NULL;
	}
	if (curr && curr->type == ARG)
	{
		add_right_child(&data->root->right, curr, data->root->type);
		if (!data->root->right)
		{
			data->mem_error = 1;
			return ;
		}
		if (!data->root->right->args)
		{
			data->mem_error = 1;
			return ;
		}
	}
	if (curr && !data->mem_error
		&& ((perms(data, data->root->right->args[0], data->root->type) == -1)
			|| data->redir_err == 1))
		set_access_err(data, data->root);
}

//sets the followup pipe as root and previous pipe as left child
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
		add_right_child(&new->right, curr, new->type);
		if (!new->right->args)
			data->mem_error = 1;
	}
	if (!data->mem_error)
		data->root = new;
}

//sets the first pipe as root node if encountered
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
		add_left_child(&data->root->left, prev_cmd, prev_cmd->type);
		if (!data->root->left->args)
			data->mem_error = 1;
		prev_cmd = NULL;
	}
	if (curr && curr->type == CMD)
	{
		add_right_child(&data->root->right, curr, data->root->type);
		if (!data->root->right->args)
			data->mem_error = 1;
	}
}
