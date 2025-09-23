/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:59:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 11:22:56 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file exec_redir.c
 * @brief Handles redirections
 */

/**
 * @brief Gets the correct parameteres for file handling
 * @param node The current node that is being handled
 * @param flags Pointer to the flags that are being set for fd-opening
 * @param file_perms Permissions that are given to a file when opened
 * @param std_fd Standard file descriptor that will be se to needed value
 * @retval 0 (No redirection found from this node)
 * @retval 1 (Permissions set correctly)
 */
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

/**
 * @brief Sets correct fd's for infile and outfile depending of the current node type
 * @param node The current node that is being handled
 * @param status Pointer to the program status structure
 */
static void	assign_node_direction(t_ast *node, t_exec_status *status)
{
	if ((node->type == RE_IN || node->type == HERE_DOC) && status->infile == -1)
		status->infile = status->temp_fd;
	else if ((node->type == RE_OUT || node->type == APPEND_OUT)
		&& status->outfile == -1 && status->redir_fail == 0)
		status->outfile = status->temp_fd;
	else
	{
		if (node->type != HERE_DOC)
		{
			close(status->temp_fd);
			status->temp_fd = -1;
		}
	}
}

/**
 * @brief Opens the files related to the current node
 * @param node The current node that is being handled
 * @param data The main data structure of the program
 * @param flags Pointer to the flags that are being set for fd-opening
 * @param file_perms Permissions that are given to a file when opened
 * @retval 1 (success)
 */
static int	open_file(t_ast *node, t_data *data, int flags, int file_perms)
{
	if (node->type == HERE_DOC)
	{
		if (data->status.here_doc_flag == 1)
			return (0);
		data->status.temp_fd = open(node->file, flags, file_perms);
		if (data->status.temp_fd < 0)
			ft_putstr_fd("open failed for heredoc\n", 2);
		data->status.here_doc_flag = 1;
	}
	else
	{
		if (data->status.redir_fail == 1)
			node->access = 0;
		if (node->right)
		{
			data->status.temp_fd = open(node->right->value, flags, file_perms);
			if (data->status.temp_fd < 0)
				ft_putstr_fd("open failed for redirection file\n", 2);
		}
	}
	return (1);
}

/**
 * @brief Sets up the redirections and goes into execution
 * @param node The current node that is being handled
 * @param data The main data structure of the program
 * @retval 0 (success)
 */
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
