/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:14:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/21 18:29:26 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_permissions_only(char *path, int type)
{
	if (type == RE_IN)
		return (access(path, R_OK));
	else if (type == RE_OUT || type == APPEND_OUT)
	{
		if (access(path, F_OK) == 0)
			return (access(path, W_OK));
		else
			return (0);
	}
	return (-1);
}

void	set_access_err(t_data *data, t_exec_status *status, t_ast *new_node)
{
	if (new_node->type != HERE_DOC)
	{
		if (data->redir_err == 0)
			(error_handler(status, data->root->cmd, strerror(errno), 1));
		new_node->access = 0;
		data->redir_err = 1;
	}
}

void	set_followup_redir(t_data *data, t_lexer *current, t_ast *new_node, t_arena *env_arena, t_exec_status *status)
{
	new_node = create_node(current->value, current->type);
	if (!new_node)
	{
		data->mem_error = 1;
		return ;
	}
	if (new_node->type == HERE_DOC)
		write_heredoc(data, env_arena, current->next->value, &new_node->file, status);
	new_node->left = data->root;
	current = current->next;
	if (current->type == ARG)
	{
		add_right_child(&new_node->right, current, new_node->type);
		if (!new_node->right->args)
		{
			data->mem_error = 1;
			return ;
		}
	}
	if (!data->mem_error && ((check_permissions_only(new_node->right->args[0], new_node->type) == -1) || data->redir_err == 1))
		set_access_err(data, status, new_node);
	if (!data->mem_error)
		data->root = new_node;
}

//sets redirection as root if no previous roots
void	set_redir_root(t_data *data, t_lexer *prev_cmd, t_lexer *current, t_arena *env_arena, t_exec_status *status)
{
	data->root = create_node(current->value, current->type);
	if (!data->root)
	{
		data->mem_error = 1;
		return ;
	}
	if (data->root->type == HERE_DOC)
		write_heredoc(data, env_arena, current->next->value, &data->root->file, status);
	current = current->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd, prev_cmd->type);
		if (!data->root->left->args)
			data->mem_error = 1;
		prev_cmd = NULL;
	}
	if (current->type == ARG)
	{
		add_right_child(&data->root->right, current, data->root->type);
		if (!data->root->right->args)
			data->mem_error = 1;
	}
	if (!data->mem_error && ((check_permissions_only(data->root->right->args[0], data->root->type) == -1)
		|| data->redir_err == 1))
		set_access_err(data, status, data->root);
}

//sets the followup pipe as root and previous pipe as left child
void	set_followup_pipe(t_data *data, t_lexer *current, t_ast *new_node)
{
	new_node = create_node(current->value, current->type);
	if (!new_node)
	{
		data->mem_error = 1;
		return ;
	}
	new_node->left = data->root;
	current = current->next;
	if (current->type == CMD)
	{
		add_right_child(&new_node->right, current, new_node->type);
		if (!new_node->right->args)
			data->mem_error = 1;
	}
	if (!data->mem_error)
		data->root = new_node;
}

//sets the first pipe as root node if encountered
void	set_first_pipe(t_data *data, t_lexer *current, t_lexer *prev_cmd)
{
	data->root = create_node(current->value, current->type);
	if (!data->root)
	{
		data->mem_error = 1;
		return ;
	}
	current = current->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd, prev_cmd->type);
		if (!data->root->left->args)
			data->mem_error = 1;
		prev_cmd = NULL;
	}
	if (current->type == CMD)
	{
		add_right_child(&data->root->right, current, data->root->type);
		if (!data->root->right->args)
			data->mem_error = 1;
	}
}

void	place_pipe(t_data *data ,t_lexer *current, t_ast *new_node, t_lexer *prev_cmd, int *redir_status)
{
	if (current->type == PIPE && data->root == NULL)
	{
		if (redir_status)
			data->redir_err = 0;
		set_first_pipe(data, current, prev_cmd);
	}
	else if (current->type == PIPE && data->root != NULL)
	{
		if (redir_status)
			data->redir_err = 0;
		set_followup_pipe(data, current, new_node);
	}
}

void	place_redir(t_data *data ,t_lexer *current, t_ast *new_node, t_lexer *prev_cmd, t_arena *env_arena, t_exec_status *status, int *redir_status)
{
	if ((current->type == RE_OUT || current->type == RE_IN
		|| current->type == APPEND_OUT || current->type == HERE_DOC) && data->root == NULL)
	{
		set_redir_root(data, prev_cmd, current, env_arena, status);
		if (data->redir_err)
			*redir_status = 1;
	}
	else if ((current->type == RE_OUT || current->type == RE_IN
		|| current->type == APPEND_OUT || current->type == HERE_DOC) && data->root != NULL)
	{
		set_followup_redir(data, current, new_node, env_arena, status);
		if (data->redir_err)
			*redir_status = 1;
	}
}

//makes a tree with pipes or redirections or both
void	set_complex_tree(t_data *data, t_arena *env_arena, t_exec_status *status)
{
	t_lexer	*current;
	t_lexer	*prev_cmd;
	t_ast	*new_node;
	int		redir_status;

	redir_status = 0;
	current = *data->lexed_list;
	prev_cmd = NULL;
	new_node = NULL;
	while (current)
	{
		if (current->type == CMD)
			prev_cmd = current;
		place_pipe(data, current, new_node, prev_cmd, &redir_status);
		place_redir(data, current, new_node, prev_cmd, env_arena, status, &redir_status);
		if (data->mem_error == 1)
			return ;
		if (current)
			current = current->next;
	}
	if (redir_status)
		data->redir_err = 1;
}
