/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:14:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/16 15:01:16 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>

int check_permissions_only(char *path, int type)
{
	if (type == RE_IN)
		return (access(path, R_OK));  // Returns 0 if readable, -1 if not
	else if (type == RE_OUT || type == APPEND_OUT)
	{
		// If file exists, check if writable
		if (access(path, F_OK) == 0)
			return (access(path, W_OK));  // Returns 0 if writable, -1 if not
		else
			return (0); // File doesn't exist — but that's OK, no permission check needed
	}
	return (-1); // Invalid redirection type
}

static char	*make_heredoc(t_arena *env_arena, char *delimiter)
{
	char	*heredoc_file;

	// `heredoc_file` holds the full path, e.g., "/tmp/temp"
	write_heredoc(env_arena, delimiter, &heredoc_file);
	// Now you can open(heredoc_file), dup2 it into stdin, etc.
	return (heredoc_file);
}

void	set_followup_redir(t_data *data, t_lexer *current, t_ast *new_node, t_arena *env_arena, t_exec_status *status)
{
	new_node = create_node(current->value, current->type);
	if (new_node->type == HERE_DOC)
		new_node->file = make_heredoc(env_arena, current->next->value);
	new_node->left = data->root;
	current = current->next;
	if (current->type == ARG)
		add_right_child(&new_node->right, current, new_node->type);
	if ((check_permissions_only(new_node->right->args[0], new_node->type) == -1) || data->redir_err == 1)
	{
		if (data->redir_err == 0)
			(error_handler(status, strerror(errno), 1));
		new_node->access = 0;
		data->redir_err = 1;
	}
	data->root = new_node;
}

//sets redirection as root if no previous roots
void	set_redir_root(t_data *data, t_lexer *prev_cmd, t_lexer *current, t_arena *env_arena, t_exec_status *status)
{
	data->root = create_node(current->value, current->type);
	if (data->root->type == HERE_DOC)
		data->root->file = make_heredoc(env_arena, current->next->value);
	current = current->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd, prev_cmd->type);
		prev_cmd = NULL;
	}
	if (current->type == ARG)
		add_right_child(&data->root->right, current, data->root->type);
	if ((check_permissions_only(data->root->right->args[0], data->root->type) == -1) || data->redir_err == 1)
	{
		if (data->redir_err == 0)
			(error_handler(status, strerror(errno), 1));
		data->root->access = 0;
		data->redir_err = 1;
	}
}

//sets the followup pipe as root and previous pipe as left child
void	set_followup_pipe(t_data *data, t_lexer *current, t_ast *new_node)
{
	new_node = create_node(current->value, current->type);
	new_node->left = data->root;
	current = current->next;
	if (current->type == CMD)
		add_right_child(&new_node->right, current, new_node->type);
	data->root = new_node;
}

//sets the first pipe as root node if encountered
void	set_first_pipe(t_data *data, t_lexer *current, t_lexer *prev_cmd)
{
	data->root = create_node(current->value, current->type);
	current = current->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd, prev_cmd->type);
			prev_cmd = NULL;
	}
	if (current->type == CMD)
		add_right_child(&data->root->right, current, data->root->type);
}

//makes a tree with pipes or redirections or both
void	set_complex_tree(t_data *data, t_arena *env_arena, t_exec_status *status)
{
	t_lexer	*current;
	t_lexer	*prev_cmd;
	t_ast	*new_node;

	current = *data->lexed_list;
	prev_cmd = NULL;
	new_node = NULL;
	while (current)
	{
		if (current->type == CMD)
			prev_cmd = current;
		else if (current->type == PIPE && data->root == NULL)
			set_first_pipe(data, current, prev_cmd);
		else if (current->type == PIPE && data->root != NULL)
			set_followup_pipe(data, current, new_node);
		else if ((current->type == RE_OUT || current->type == RE_IN
			|| current->type == APPEND_OUT || current->type == HERE_DOC) && data->root == NULL)
			set_redir_root(data, prev_cmd, current, env_arena, status);
		else if ((current->type == RE_OUT || current->type == RE_IN
			|| current->type == APPEND_OUT || current->type == HERE_DOC) && data->root != NULL)
			set_followup_redir(data, current, new_node, env_arena, status);
		if (current)
			current = current->next;
	}
}
