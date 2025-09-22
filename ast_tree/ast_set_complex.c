/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:14:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:44:12 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file ast_set_complex.c
 * @brief Makes the complex AST
 */

/**
 * @brief Checks if the iterator is inside quotes
 * @param ast Contains important values for token value processing (like a toolkit)
 * @param value Value that is being handled
 */
void	handle_quote(t_utils *ast, char *value)
{
	if (value[(*ast).i] == (*ast).quote)
	{
		(*ast).i++;
		(*ast).quote = 0;
	}
	else
		(*ast).cleaned_value[(*ast).j++] = value[(*ast).i++];
}

/**
 * @brief Removes all quotes of a value
 * @param value Value that is being handled
 */
char	*remove_quotes(char *value)
{
	t_utils	ast;

	ft_memset(&ast, 0, sizeof(t_utils));
	ast.len = count_new_len(value);
	ast.cleaned_value = malloc(sizeof(char) * (ast.len + 1));
	if (!ast.cleaned_value)
		return (NULL);
	while (value[ast.i])
	{
		if (!ast.quote && (value[ast.i] == '\'' || value[ast.i] == '"'))
		{
			ast.quote = value[ast.i];
			ast.i++;
		}
		else if (ast.quote)
			handle_quote(&ast, value);
		else
			ast.cleaned_value[ast.j++] = value[ast.i++];
	}
	ast.cleaned_value[ast.j] = '\0';
	return (ast.cleaned_value);
}

/**
 * @brief Places pipe as the root node
 * @param data The entire main data structure
 * @param curr Current node in the lexed linked list
 * @param new Current node of the AST
 * @param prev_cmd The previous command that has been encountered
 */
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

/**
 * @brief Places RE_IN, RE_OUT, APPEND_OUT or HERE_DOC as the root node (either first root node or in the place of an existing one)
 * @param data The entire main data structure
 * @param curr Current node in the lexed linked list
 * @param new Current node of the AST
 * @param prev_cmd The previous command that has been encountered
 */
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

/**
 * @brief Creates the complex AST
 * @param data The entire main data structure
 */
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
