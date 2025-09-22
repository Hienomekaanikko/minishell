/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:26:47 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/12 10:46:02 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file ast_set_complex.c
 * @brief Makes the complex AST
 */

/**
 * @brief Checks if file is a directory
 * @param path Path of the file
 * @retval 0 (Is not directory)
 * @retval 1 (Is a directory)
 */
int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

/**
 * @brief Checks permissions/problems in a file
 * @param data The main data structure
 * @param path Path to the file
 * @param type Type of the node to make correct checks
 * @retval < 0 (Problem with a file)
 * @retval 0 (No problem with the file)
 */
int	perms(t_data *data, char *path, int type)
{
	if (path[0] == '$' && path[1] && type != HERE_DOC)
	{
		(error(&data->status, path, AMB, 1));
		data->mem_error = 2;
		return (-2);
	}
	if (is_directory(path))
	{
		error(&data->status, path, ISDIR, 1);
		data->redir_err = 1;
		return (-2);
	}
	else if (type == RE_IN)
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

int	process_token(t_utils *ast, t_ast *node, t_lexer **temp, int *skip_next)
{
	if (*skip_next)
	{
		*skip_next = 0;
		*temp = (*temp)->next;
		return (1);
	}
	if ((*temp)->type == RE_IN || (*temp)->type == RE_OUT
		|| (*temp)->type == APPEND_OUT || (*temp)->type == HERE_DOC)
	{
		*skip_next = 1;
		*temp = (*temp)->next;
		return (1);
	}
	if ((*temp)->type == ARG || (*temp)->type == CMD)
	{
		if (!allocate_arguments(ast, node, temp))
			return (0);
		return (1);
	}
	*temp = (*temp)->next;
	return (1);
}

void	add_args(t_utils *ast, t_ast *node, t_lexer *current)
{
	t_lexer	*temp;
	int		skip_next;

	temp = current;
	skip_next = 0;
	ast->argument_amount = count_size(temp);
	node->args = malloc((ast->argument_amount + 1) * sizeof(char *));
	if (!node->args)
		return ;
	while (temp && temp->type != PIPE)
	{
		if (!process_token(ast, node, &temp, &skip_next))
			return ;
	}
	node->args[ast->i] = NULL;
}
