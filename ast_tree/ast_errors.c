/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:37:03 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/12 10:36:27 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file ast_errors.c
 * @brief Handles errors during AST creation
 */

/**
 * @brief Checks if the creation of a child node failed
 * @param data The main data structure
 * @param child The child node
 * @retval 1 (has failed)
 * @retval 2 (success)
 */
int	is_child_failure(t_data *data, t_ast *child)
{
	if (!child)
	{
		data->mem_error = 1;
		return (1);
	}
	if (!child->args)
	{
		data->mem_error = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if the creation of a node failed
 * @param data The main data structure
 * @retval 1 (has failed)
 * @retval 2 (success)
 */
int	node_error(t_ast *node)
{
	if (!node->value)
	{
		free(node);
		return (1);
	}
	return (0);
}

/**
 * @brief Checks if an error message has been assigned to data->status.msg
 * @param data The main data structure
 */
void	ast_error_check(t_data *data)
{
	if (data->status.msg && data->status.path)
	{
		if (ft_strncmp(data->status.msg, "No such file or directory", 26) == 0)
			error(&data->status, data->status.path, NOFILE, 1);
		else if (ft_strncmp(data->status.msg, "Permission denied", 18) == 0)
			error(&data->status, data->status.path, NOPERM, 1);
		data->status.msg = NULL;
		data->status.path = NULL;
	}
	if (data->redir_status)
		data->redir_err = 1;
}

/**
 * @brief Sets correct access status to an unaccessable file and sets data->redir_err to 1 (true)
 * @param data The main data structure
 * @param new Node where access error was found from
 */
void	set_access_err(t_data *data, t_ast *new)
{
	if (new->type != HERE_DOC)
	{
		if (data->redir_err == 0)
		{
			data->status.msg = strerror(errno);
			if (set_mem_error(data, data->status.msg))
				return ;
			data->status.path = new->right->args[0];
			if (set_mem_error(data, data->status.path))
				return ;
		}
		new->access = 0;
		data->redir_err = 1;
	}
}
