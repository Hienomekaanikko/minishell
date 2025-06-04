/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 11:46:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 15:05:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path_permissions(t_data *data, char *path)
{
	struct stat	path_stat;

	if ((path[0] == '.' && path[1] == '/') || path[0] == '/')
	{
		if (stat(path, &path_stat) == -1)
		{
			error(&data->status, path, NOFILE, 127);
			return (127);
		}
		if (S_ISDIR(path_stat.st_mode))
		{
			error(&data->status, path, ISDIR, 126);
			return (126);
		}
		if (access(path, X_OK) == -1)
		{
			error(&data->status, path, NOPERM, 126);
			return (126);
		}
	}
	return (0);
}

void	close_fds(t_exec_status *exec_status)
{
	if (exec_status->outfile != -1)
		close(exec_status->outfile);
	if (exec_status->saved_stdin != -1)
		close(exec_status->saved_stdin);
	if (exec_status->saved_stdout != -1)
		close(exec_status->saved_stdout);
	if (exec_status->temp_fd != -1)
		close(exec_status->temp_fd);
}

void	restore_orig_fd(t_data *data)
{
	if (data->status.outfile != -1 && data->status.saved_stdout != -1)
	{
		if (dup2(data->status.saved_stdout, STDOUT_FILENO) == -1)
			perror("dup2 restore stdout failed");
		close(data->status.saved_stdout);
		data->status.saved_stdout = -1;
		close(data->status.outfile);
		data->status.outfile = -1;
	}
	if (data->status.infile != -1 && data->status.saved_stdin != -1)
	{
		if (dup2(data->status.saved_stdin, STDIN_FILENO) == -1)
			perror("dup2 restore stdin failed");
		close(data->status.saved_stdin);
		data->status.saved_stdin = -1;
		close(data->status.infile);
		data->status.infile = -1;
	}
	if (data->status.temp_fd != -1)
	{
		close(data->status.temp_fd);
		data->status.temp_fd = -1;
	}
}

void	save_orig_fd(t_data *data)
{
	if (data->status.infile != -1)
	{
		data->status.saved_stdin = dup(STDIN_FILENO);
		if (data->status.saved_stdin == -1)
			perror("dup saved_stdin failed");
		else if (dup2(data->status.infile, STDIN_FILENO) == -1)
			perror("dup2 infile to stdin failed");
	}
	if (data->status.outfile != -1)
	{
		data->status.saved_stdout = dup(STDOUT_FILENO);
		if (data->status.saved_stdout == -1)
			perror("dup saved_stdout failed");
		else if (dup2(data->status.outfile, STDOUT_FILENO) == -1)
			perror("dup2 outfile to stdout failed");
	}
}
