#include "minishell.h"

static int	update_pwd(t_data *data)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error(&data->status, "cd", NOFILE, 1));
	arena_set_env(data, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

static int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*path;

	path = NULL;
	if (count_args(args) > 2)
		return (error(&data->status, "cd", TOO_MANY, 1));
	if (args[1])
		path = ft_strdup(args[1]);
	if (!path)
	{
		path = arena_getenv(data, data->env_arena, "HOME");
		if (data->mem_error)
			return (0);
		if (!path)
			return (error(&data->status, "cd: HOME", NOTSET, 1));
	}
	if (chdir(path) == -1)
	{
		free(path);
		return (error(&data->status, "cd", NOFILE, 1));
	}
	if(update_pwd(data))
		return (1);
	free(path);
	return (0);
}