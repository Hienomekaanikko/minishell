#include "minishell.h"

int	builtin_export(t_arena *env_arena, t_exec_status *status, char **args, t_data *data)
{
	size_t		i;
	char	*key;
	char	*eq;

	if (!args[1])
	{
		i = 0;
		while (i < env_arena->ptrs_in_use)
		{
			eq = ft_strchr(env_arena->ptrs[i], '=');
			ft_putstr_fd("declare -x ", 1);
			if (eq)
			{
				write(1, env_arena->ptrs[i], eq - env_arena->ptrs[i]);
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(eq + 1, 1);
				ft_putstr_fd("\"\n", 1);
			}
			else
			{
				ft_putstr_fd(env_arena->ptrs[i], 1);
				ft_putstr_fd("\n", 1);
			}
			i++;
		}
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			key = ft_strndup(args[i], ft_strchr(args[i], '=') - args[i]);
			if (!key)
				return (error_handler(status, "export", strerror(errno), 1));
		}
		else
			key = args[i];
		if (!is_valid_env_name(key))
		{
			if (ft_strchr(args[i], '='))
				free(key);
			return (error_handler(status, args[0], "not a valid identifier", 1));
		}
		if (ft_strchr(args[i], '='))
		{
			if (arena_set_env(env_arena, key, ft_strchr(args[i], '=') + 1, status) == -1)
			{
				free(key);
				return (error_handler(status,"export", strerror(errno), 1));
			}
			free(key);
		}
		else
		{
			char *value = is_declared(data, key);
			if (value)
			{
				arena_set_env(env_arena, key, value, status);
				free(value);
			}
			else
			{
				arena_set_env(env_arena, key, NULL, status);
			}
		}
		i++;
	}
	return (0);
}