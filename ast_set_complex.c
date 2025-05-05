/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:14:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 18:18:26 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_heredoc(t_arena *env_arena, char *delimiter)
{
	char	*heredoc_file;

	// `heredoc_file` holds the full path, e.g., "/tmp/temp"
	write_heredoc(env_arena, delimiter, &heredoc_file);
	// Now you can open(heredoc_file), dup2 it into stdin, etc.
	return (heredoc_file);
}

void	set_followup_redir(t_data *data, t_lexer *current, t_ast *new_node, t_arena *env_arena)
{
	new_node = create_node(current->value, current->type);
	if (new_node->type == HERE_DOC)
		new_node->file = make_heredoc(env_arena, current->next->value);
	new_node->left = data->root;
	current = current->next;
	if (current->type == ARG)
		add_right_child(&new_node->right, current);
	data->root = new_node;
}

//sets redirection as root if no previous roots
void	set_redir_root(t_data *data, t_lexer *prev_cmd, t_lexer *current, t_arena *env_arena)
{
	data->root = create_node(current->value, current->type);
	if (data->root->type == HERE_DOC)
		data->root->file = make_heredoc(env_arena, current->next->value);
	current = current->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd);
		prev_cmd = NULL;
	}
	if (current->type == ARG)
		add_right_child(&data->root->right, current);
}

//sets the followup pipe as root and previous pipe as left child
void	set_followup_pipe(t_data *data, t_lexer *current, t_ast *new_node)
{
	new_node = create_node(current->value, current->type);
	new_node->left = data->root;
	current = current->next;
	if (current->type == CMD)
		add_right_child(&new_node->right, current);
	data->root = new_node;
}

//sets the first pipe as root node if encountered
void	set_first_pipe(t_data *data, t_lexer *current, t_lexer *prev_cmd)
{
	data->root = create_node(current->value, current->type);
	current = current->next;
	if (prev_cmd != NULL)
	{
		add_left_child(&data->root->left, prev_cmd);
			prev_cmd = NULL;
	}
	if (current->type == CMD)
		add_right_child(&data->root->right, current);
}

//makes a tree with pipes or redirections or both
void	set_complex_tree(t_data *data, t_arena *env_arena)
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
			set_redir_root(data, prev_cmd, current, env_arena);
		else if ((current->type == RE_OUT || current->type == RE_IN
			|| current->type == APPEND_OUT || current->type == HERE_DOC) && data->root != NULL)
			set_followup_redir(data, current, new_node, env_arena);
		if (current)
			current = current->next;
	}
}
