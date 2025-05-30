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
		error_handler(status, "PATH: not set", 127);
		return (NULL);
	}
	paths = ft_split(path_str, ':');
	if (!paths)
	{
		error_handler(status, "memory: allocation failed", 1);
		return (NULL);
	}
	executable = search_paths(paths, cmd);
	ft_free_split(paths);
	return (executable);
}

char	*find_executable(t_ast *node, t_arena *env_arena)
{
	char *executable;

	executable = try_path(node->cmd, SECURE_PATH, NULL);
	if (executable)
		return (executable);
	executable = try_path(node->cmd, arena_getenv(env_arena, "PATH"), NULL);
	return (executable);
}