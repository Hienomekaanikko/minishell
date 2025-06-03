/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:25:12 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 09:31:25 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_ast *root)
{
	if (root == NULL)
		return ;
	free_ast(root->left);
	free_ast(root->right);
	if (root->args)
		ft_free_split(root->args);
	if (root->cmd)
		free(root->cmd);
	if (root->path)
		free(root->path);
	if (root->file)
	{
		unlink(root->file);
		free(root->file);
	}
	free(root);
}

void	free_lexed_list(t_lexer *start)
{
	t_lexer	*curr;
	t_lexer	*tmp;

	curr = start;
	while (curr)
	{
		tmp = curr->next;
		if (curr->value)
			free(curr->value);
		free (curr);
		curr = tmp;
	}
}

static void	free_var_list(t_var *start)
{
	t_var	*curr;
	t_var	*tmp;

	curr = start;
	tmp = NULL;
	while (curr)
	{
		tmp = curr->next;
		if (curr->key)
			free(curr->key);
		if (curr->value)
			free(curr->value);
		free(curr);
		curr = tmp;
	}
}

void	destroy_memory(t_data *data)
{
	close_all_fds(data);
	if (data->tools)
		free(data->tools);
	if (data->temp_array)
		ft_free_split(data->temp_array);
	if (data->root)
		free_ast(data->root);
	if (data->exp)
	{
		if (data->exp->var_list)
			free_var_list(data->exp->var_list);
		free(data->exp);
	}
	if (data->lexed_list)
		free(data->lexed_list);
	if (data->input)
		free(data->input);
	clear_history();
}
