/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_executables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:05 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/21 16:34:22 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

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

static char	*search_paths(t_ast *node, char **paths)
{
	char	*executable;
	int		i;

	i = 0;
	while (paths[i])
	{
		executable = check_single_path(paths[i], node->cmd);
		if (executable)
			return (executable);
		i++;
	}
	return (NULL);
}

static char	*try_path(t_data *data, t_ast *node, char *path_str)
{
	char	**paths;
	char	*executable;

	if (!path_str)
	{
		error_handler(data, "PATH", "not set", 127);
		return (NULL);
	}
	paths = ft_split(path_str, ':');
	if (!paths)
	{
		error_handler(data, "memory", "allocation failed", 1);
		return (NULL);
	}
	executable = search_paths(node, paths);
	ft_free_split(paths);
	return (executable);
}

char	*find_executable(t_data *data, t_ast *node)
{
	char	*executable;
	char	*path_env;

	executable = try_path(data, node, SECURE_PATH);
	if (executable)
		return (executable);
	path_env = arena_getenv(data, "PATH");
	executable = try_path(data, node, path_env);
	free(path_env);
	return (executable);
}
