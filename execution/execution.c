/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:10:49 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 11:58:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_all(t_data *data)
{
	arena_free(data->env_arena);
	destroy_memory(data);
}

static void	child_exit(t_data *data, int status)
{
	free_all(data);
	if (status == 1)
		exit(data->status.exit_code);
	else if (status == 2)
		exit(error(&data->status, "''", NOCMD, 127));
}

int	built_ins(t_ast *node, t_data *data)
{
	if (ft_strncmp(node->value, "echo", 5) == 0)
		return (builtin_echo(node->args, &data->status));
	else if (ft_strncmp(node->value, "cd", 3) == 0)
		return (builtin_cd(node->args, data));
	else if (ft_strncmp(node->value, "pwd", 4) == 0)
		return (builtin_pwd(data));
	else if (ft_strncmp(node->value, "export", 7) == 0)
		return (builtin_export(data, node->args));
	else if (ft_strncmp(node->value, "unset", 6) == 0)
		return (builtin_unset(data, node->args));
	else if (ft_strncmp(node->value, "env", 4) == 0)
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
		if (check_path_permissions(data, node->value) > 0)
			child_exit(data, 1);
		close_fds(&data->status);
		node->path = find_executable(node, data);
		if (!node->path)
			child_exit(data, 1);
		execve(node->path, node->args, data->env_arena->ptrs);
		child_exit(data, 2);
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
