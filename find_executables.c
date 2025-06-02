/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:05 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/02 16:19:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_single_path(char *dir, char *cmd, t_exec_status *status)
{
	char	*path;
	char	*full_path;

	path = ft_strjoin(dir, "/");
	if (!path)
	{
		error(status, "malloc", MALLOC, 1);
		return (NULL);
	}
	full_path = ft_strjoin(path, cmd);
	free(path);
	if (!full_path)
	{
		error(status, "malloc", MALLOC, 1);
		return (NULL);
	}
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	*search_paths(t_exec_status *status, char **paths, char *cmd)
{
	char	*executable;
	int		i;

	i = 0;
	while (paths[i])
	{
		executable = check_single_path(paths[i], cmd, status);
		if (executable)
			return (executable);
		if (status->exit_code == 1)
			return (NULL);
		i++;
	}
	return (NULL);
}

static char	*try_path(char *cmd, char *path_str, t_exec_status *status)
{
	char	**paths;
	char	*executable;

	if (!path_str)
	{
		error(status, cmd, NOFILE, 127);
		return (NULL);
	}
	paths = ft_split(path_str, ':');
	if (!paths)
	{
		error(status, "malloc", MALLOC, 1);
		return (NULL);
	}
	executable = search_paths(status, paths, cmd);
	if (status->exit_code == 1)
	{
		ft_free_split(paths);
		return (NULL);
	}
	ft_free_split(paths);
	return (executable);
}

char	*find_executable(t_ast *node, t_data *data)
{
	char	*executable;
	char	*path_env;

	if (ft_strchr(node->cmd, '/'))
	{
		if (access(node->cmd, X_OK) == 0)
			return (node->cmd);
		else
		{
			error(&data->status, node->cmd, NOFILE, 126);
			return (NULL);
		}
	}
	path_env = arena_getenv(data, data->env_arena, "PATH");
	if (data->mem_error)
		return (NULL);
	executable = try_path(node->cmd, path_env, &data->status);
	free(path_env);
	if (!executable && data->status.exit_code == 0)
		error(&data->status, node->cmd, NOCMD, 127);
	if (data->status.exit_code == 1)
		data->mem_error = 1;
	return (executable);
}

