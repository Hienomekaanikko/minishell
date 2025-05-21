/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:44 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/21 16:35:25 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_ins(t_ast *node, t_data *data)
{
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(data, node->args));
	else if (ft_strncmp(node->cmd, "cd", 3) == 0)
		return (builtin_cd(data, node->args));
	else if (ft_strncmp(node->cmd, "pwd", 4) == 0)
		return (builtin_pwd(data));
	else if (ft_strncmp(node->cmd, "export", 7) == 0)
		return (builtin_export(data, node->args));
	else if (ft_strncmp(node->cmd, "unset", 6) == 0)
		return (builtin_unset(data, node->args));
	else if (ft_strncmp(node->cmd, "env", 4) == 0)
		return (builtin_env(data));
	return (-1);
}

void check_path_permissions(t_data *data)
{
	struct stat path_stat;

	if ((data->root->cmd[0] == '.' && data->root->cmd[1] == '/') || data->root->cmd[0] == '/')
	{
		if (stat(data->root->cmd, &path_stat) == -1)
			exit(error_handler(data, data->root->cmd, strerror(errno), 127));
		if (S_ISDIR(path_stat.st_mode))
			exit(error_handler(data, data->root->cmd, "Is a directory", 126));
		if (access(data->root->cmd, X_OK) == -1)
			exit(error_handler(data, data->root->cmd, "Permission denied", 126));
		return;
	}
}
void	close_fds(t_data *data)
{
	if (data->status->saved_stdout != -1)
	{
		close(data->status->saved_stdout);
		data->status->saved_stdout = -1;
	}
	if (data->status->temp_fd != -1)
	{
		close(data->status->temp_fd);
		data->status->temp_fd = -1;
	}
}
int	executables(t_data *data, t_ast *node)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
		return (error_handler(data, "fork", "failed", 1));
	if (pid == 0)
	{
		setup_child_signals();
		check_path_permissions(data);
		path = find_executable(data, node);
		if (!path)
			exit(error_handler(data, data->root->cmd, "command not found", 127));
		if (data->status->outfile != -1)
		{
			if (dup2(data->status->outfile, 1) == -1)
				exit(handle_redirection_error(data));
			close(data->status->outfile);
			data->status->outfile = -1;
		}
		if (data->status->infile != -1)
		{
			if (dup2(data->status->infile, 0) == -1)
				exit(handle_redirection_error(data));
			close(data->status->infile);
			data->status->infile = -1;
		}
		close_fds(data);
		execve(path,node->args, data->env_arena->ptrs);
		free(path);
		exit(error_handler(data, data->root->cmd, "command not found", 127));
	}
	else if (pid > 0)
	{
		data->status->pid = pid;
		wait_process(data, pid);
	}
	return (0);
}

int	execute_command(t_data *data, t_ast *node)
{
	if (!node)
		return (error_handler(data, "syntax error", "invalid command", 1));
	if (node->type == RE_OUT || node->type == APPEND_OUT || node->type == RE_IN || node->type == HERE_DOC)
		return (exec_redir(data, node));
	else if (node->type == PIPE && data->status->redir_fail == 0)
		return (exec_pipe(data, node));
	else
	{
		if (data->status->outfile != -1)
		{
			data->status->saved_stdout = dup(STDOUT_FILENO);
			dup2(data->status->outfile, STDOUT_FILENO);
		}
		if (built_ins(node, data) == -1)
		{
			if (data->status->redir_fail == 0)
			{
				if (executables(data, node) == -1)
					return (error_handler(data, "WIP: exec", "command not found", 127));
			}
		}
		if (data->status->outfile != -1 && data->status->saved_stdout != -1)
		{
			dup2(data->status->saved_stdout, STDOUT_FILENO);
			close(data->status->saved_stdout);
			close(data->status->outfile);
			if (data->status->temp_fd != -1)
				close(data->status->temp_fd);
		}
	}
	if (data->status->redir_fail == 1)
		data->status->exit_code = 1;
	return (0);
}
//
//echo <"./test_files/infile" <missing <"./test_files/infile"
