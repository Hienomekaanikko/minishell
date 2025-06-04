/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:59:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 14:30:37 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_params(t_ast *node, int *flags, int *file_perms, int *std_fd)
{
	if (node->type == RE_OUT)
	{
		*flags = O_WRONLY | O_CREAT | O_TRUNC;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else if (node->type == RE_IN || node->type == HERE_DOC)
	{
		*flags = O_RDONLY;
		*file_perms = 0;
		*std_fd = STDIN_FILENO;
	}
	else if (node->type == APPEND_OUT)
	{
		*flags = O_WRONLY | O_CREAT | O_APPEND;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else
		return (0);
	return (1);
}

static void	assign_node_direction(t_ast *node, t_exec_status *status)
{
	if ((node->type == RE_IN || node->type == HERE_DOC) && status->infile == -1)
		status->infile = status->temp_fd;
	if ((node->type == RE_OUT || node->type == APPEND_OUT)
		&& status->outfile == -1 && status->redir_fail == 0)
		status->outfile = status->temp_fd;
}

static int	open_file(t_ast *node, t_data *data, int flags, int file_perms)
{
	if (node->type == HERE_DOC)
	{
		if (data->status.here_doc_flag == 1)
			return (0);
		data->status.temp_fd = open(node->file, flags, file_perms);
		data->status.here_doc_flag = 1;
	}
	else
	{
		if (data->status.redir_fail == 1)
			node->access = 0;
		if (node->right)
			data->status.temp_fd = open(node->right->cmd, flags, file_perms);
	}
	return (1);
}

int	exec_redir(t_ast *node, t_data *data)
{
	int	flags;
	int	file_perms;
	int	std_fd;

	if (node->access == 0)
	{
		data->status.redir_fail = 1;
		return (execute_command(node->left, data));
	}
	if (!get_params(node, &flags, &file_perms, &std_fd))
		return (0);
	open_file(node, data, flags, file_perms);
	if (data->status.temp_fd == -1)
		return (execute_command(node->left, data));
	else
	{
		assign_node_direction(node, &data->status);
		return (execute_command(node->left, data));
	}
	return (0);
}
