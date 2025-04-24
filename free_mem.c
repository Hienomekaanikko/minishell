/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:25:12 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 16:18:19 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_ast *root)
{
	t_ast	*curr;
	t_ast	*tmp;

	curr = root;
	while (curr)
	{
		tmp = curr->left;
		if (curr->right)
			free(curr->right);
		free(curr);
		curr = tmp;
	}
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
	if (data->exp)
	{
		if (data->exp->var_list)
		{
			free_var_list(data->exp->var_list);
			data->exp->var_list = NULL;
		}
		free(data->exp);
	}
	if (data->lexed_list)
	{
		free_lexed_list(*data->lexed_list);
		free(data->lexed_list);
	}
	if (data->input)
		free(data->input);
}
