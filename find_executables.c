/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:05 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/29 16:13:37 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_single_path(char *dir, char *cmd)
{
	char	*path;
	char	*full_path;

	path = ft_strjoin(dir, "/");
	if (!path)
		return (NULL);
	full_path = ft_strjoin(path, cmd);
	free(path);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	*search_paths(char **paths, char *cmd)
{
	char	*executable;
	int		i;

	i = 0;
	while (paths[i])
	{
		executable = check_single_path(paths[i], cmd);
		if (executable)
			return (executable);
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
		error_handler(status, cmd, NOFILE, 127);
		return (NULL);
	}
	paths = ft_split(path_str, ':');
	if (!paths)
	{
		error_handler(status, "malloc", MALLOC, 1);
		return (NULL);
	}
	executable = search_paths(paths, cmd);
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
			return (ft_strdup(node->cmd));
		else
		{
			error_handler(&data->status, node->cmd, NOFILE, 126);
			return (NULL);
		}
	}
	path_env = arena_getenv(data->env_arena, "PATH");
	executable = try_path(node->cmd, path_env, &data->status);
	free(path_env);
	if (!executable && data->status.exit_code == 0)
		error_handler(&data->status, node->cmd, NOCMD, 127);
	return (executable);
}

