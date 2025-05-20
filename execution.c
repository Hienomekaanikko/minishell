/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:44 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/20 11:26:03 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	built_ins(t_data *data)
{
	if (ft_strncmp(data->root->cmd, "echo", 5) == 0)
		return (builtin_echo(data));
	else if (ft_strncmp(data->root->cmd, "cd", 3) == 0)
		return (builtin_cd(data));
	else if (ft_strncmp(data->root->cmd, "pwd", 4) == 0)
		return (builtin_pwd(data));
	else if (ft_strncmp(data->root->cmd, "export", 7) == 0)
		return (builtin_export(data));
	else if (ft_strncmp(data->root->cmd, "unset", 6) == 0)
		return (builtin_unset(data));
	else if (ft_strncmp(data->root->cmd, "env", 4) == 0)
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

int	executables(t_data *data)
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
		path = find_executable(data);
		if (!path)
			exit(error_handler(data, data->root->cmd, "command not found", 127));
		execve(path, data->root->args, data->env_arena->ptrs);
		free(path);
		exit(error_handler(data, data->root->cmd, "command not found", 127));
	}
	else if (pid > 0)
	{
		data->status->pid = pid;
		wait_process(pid, data);
	}
	return (0);
}

int	execute_command(t_data *data)
{
	if (!data->root)
		return (error_handler(data, data->root->cmd, "command not found", 127));
	//print_node_structure(node); //DEBUG
	if (data->root->type == RE_OUT || data->root->type == APPEND_OUT || data->root->type == RE_IN || data->root->type == HERE_DOC)
		return (exec_redir(data));
	else if (data->root->type == PIPE && data->status->redir_fail == 0)
		return (exec_pipe(data));
	else if (built_ins(data) == -1)
	{
		if (data->status->redir_fail == 0)
		{
			if (executables(data) == -1)
				return (error_handler(data, data->root->cmd, "command not found", 127));
		}
	}
	if (data->status->redir_fail == 1)
		data->status->exit_code = 1;
	return (0);
}
//
//echo <"./test_files/infile" <missing <"./test_files/infile"
