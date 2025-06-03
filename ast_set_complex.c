/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:14:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 12:39:02 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	place_pipe(t_data *data, t_lexer *curr, t_ast *new, t_lexer *prev_cmd)
{
	if (curr->type == PIPE && data->status.msg && data->status.path)
	{
		if (ft_strncmp(data->status.msg, "No such file or directory", 26) == 0)
			error(&data->status, data->status.path, NOFILE, 0);
		else if (ft_strncmp(data->status.msg, "Permission denied", 18) == 0)
			error(&data->status, data->status.path, NOPERM, 0);
		data->status.msg = NULL;
		data->status.path = NULL;
	}
	if (curr->type == PIPE && data->root == NULL)
	{
		if (data->redir_status)
			data->redir_err = 0;
		set_first_pipe(data, curr, prev_cmd);
	}
	else if (curr->type == PIPE && data->root != NULL)
	{
		if (data->redir_status)
			data->redir_err = 0;
		set_followup_pipe(data, curr, new);
	}
}

void	place_redir(t_data *data, t_lexer *curr, t_ast *new, t_lexer *prev_cmd)
{
	if ((curr->type == RE_OUT || curr->type == RE_IN
			|| curr->type == APPEND_OUT
			|| curr->type == HERE_DOC) && data->root == NULL)
	{
		set_redir_root(data, prev_cmd, curr);
		if (data->redir_err)
			data->redir_status = 1;
	}
	else if ((curr->type == RE_OUT || curr->type == RE_IN
			|| curr->type == APPEND_OUT
			|| curr->type == HERE_DOC) && data->root != NULL)
	{
		set_followup_redir(data, curr, new);
		if (data->redir_err)
			data->redir_status = 1;
	}
}

//makes a tree with pipes or redirections or both
void	set_complex_tree(t_data *data)
{
	t_lexer	*curr;
	t_lexer	*prev_cmd;
	t_ast	*new;

	curr = *data->lexed_list;
	prev_cmd = NULL;
	new = NULL;
	while (curr)
	{
		if (curr && curr->type == CMD)
			prev_cmd = curr;
		place_pipe(data, curr, new, prev_cmd);
		if (data->mem_error == 1)
			return ;
		place_redir(data, curr, new, prev_cmd);
		if (data->mem_error == 1 || data->redir_err == 2)
			return ;
		curr = curr->next;
	}
	ast_error_check(data);
}
