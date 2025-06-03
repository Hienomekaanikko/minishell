/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:10:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 17:53:06 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_all(t_data *data)
{
	arena_free(data->env_arena);
	destroy_memory(data);
}

int	built_ins(t_ast *node, t_data *data)
{
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, &data->status));
	else if (ft_strncmp(node->cmd, "cd", 3) == 0)
		return (builtin_cd(node->args, data));
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

int	executables(t_ast *node, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (error(&data->status, "fork", FAIL, 1));
	if (pid == 0)
	{
		setup_child_signals();
		if (check_path_permissions(data, node->cmd) > 0)
		{
			free_all(data);
			exit(data->status.exit_code);
		}
		close_fds(&data->status);
		node->path = find_executable(node, data);
		if (!node->path)
		{
			free_all(data);
			exit(data->status.exit_code);
		}
		execve(node->path, node->args, data->env_arena->ptrs);
		exit(error(&data->status, node->cmd, NOCMD, 127));
	}
	else
	{
		data->status.pid = pid;
		ignore_signals();
		wait_process(pid, &data->status);
		setup_shell_signals();
	}
	return (0);
}

int	execute_command(t_ast *node, t_data *data)
{
	if (!node)
		return (0);
	if (node->type == RE_OUT || node->type == APPEND_OUT
		|| node->type == RE_IN || node->type == HERE_DOC)
		return (exec_redir(node, data));
	else if (node->type == PIPE && data->status.redir_fail == 0)
		return (exec_pipe(node, data));
	else
	{
		save_orig_fd(data);
		if (data->status.redir_fail == 0)
		{
			if (built_ins(node, data) == -1)
				executables(node, data);
		}
		restore_orig_fd(data);
	}
	close_fds(&data->status);
	if (data->status.redir_fail == 1)
		data->status.exit_code = 1;
	return (0);
}
