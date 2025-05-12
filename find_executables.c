# include "minishell.h"

int check_file_status(const char *path, t_exec_status *status)
{
	struct stat path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (error_handler(status, path, ERR_IS_DIR));
		if (!(path_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
			return (error_handler(status, path, ERR_PERMISSION));
		return (0);
	}
	return (handle_errno_error(status, path, errno));
}

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
	if (check_file_status(full_path, NULL) == 0)
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
		error_handler(status, "PATH", ERR_NOT_SET);
		return (NULL);
	}
	paths = ft_split(path_str, ':');
	if (!paths)
	{
		handle_errno_error(status, NULL, errno);
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