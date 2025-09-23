/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:25:12 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/12 10:12:35 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file free_mem.c
 * @brief Cleanup functions for the main loop of the program
 */

/**
 * @brief Frees the memory from the AST
 * @param root Root node of the AST
 */
void	free_ast(t_ast *root)
{
	if (root == NULL)
		return ;
	free_ast(root->left);
	free_ast(root->right);
	if (root->args)
		ft_free_split(root->args);
	if (root->value)
		free(root->value);
	if (root->path)
		free(root->path);
	if (root->file)
	{
		unlink(root->file);
		free(root->file);
	}
	free(root);
}

/**
 * @brief Frees the memory from the lexed linked list
 * @param start Root node of the lexed linked list
 */
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

/**
 * @brief Frees the memory from the local variable list
 * @param start Root node of the local variable list
 */
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

/**
 * @brief Frees the memory from everywhere
 * @param data The main data structure of the program
 */
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
