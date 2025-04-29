#include "minishell.h"

int	builtin_echo(char **args, t_exec_status *status)
{
	int		newline_flag;
	int		i;

	if (!args || !args[0])
		return (2);
	i = 1;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
		i++;
	newline_flag = i;
	while(args[i])
	{
		if (ft_strncmp(args[i], "$!", 3) == 0)
			ft_putnbr_fd(status->pid, 1);
		else if (ft_strncmp(args[i], "$?", 3) == 0)
			ft_putnbr_fd(status->exit_code, 1);
		else
			ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline_flag == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	builtin_cd(char **args, t_exec_status *status)
{
	if (chdir(args[1]) == -1)
	{
		handle_exec_error(status, "cd: No such file or directory", 1);
		return (1);
	}
	return (0);
}

int	builtin_pwd(t_exec_status *status)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		handle_exec_error(status, "pwd: getcwd failed", 1);
		return (1);
	}
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}

static int	is_valid_env_name(const char *name)
{
	if (!name || !*name || ft_isdigit(*name))
		return (0);
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	builtin_export(t_arena *env_arena, t_exec_status *status, char **args)
{
	int		i;
	char	*equals;
	char	*key;
	char	*value;

	if (!args[1])
		return (builtin_env(env_arena, status));
	i = 1;
	while (args[i])
	{
		equals = ft_strchr(args[i], '=');
		if (equals)
		{
			*equals = '\0';
			key = args[i];
			value = equals + 1;
			if (arena_set_env(env_arena, key, value) == -1)
			{
				*equals = '=';
				handle_exec_error(status, "export: Memory error", 1);
				return (1);
			}
			*equals = '=';
		}
		i++;
	}
	return (0);
}



int	builtin_unset(t_arena *env_arena, t_exec_status *status, char **args)
{
	int	i;
	int	return_value;

	return_value = 0;
	i = 1;
	if (!args[1])
	{
		handle_exec_error(status, "unset: not enough arguments", 1);
		return (1);
	}
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			ft_putstr_fd("unset: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return_value = 1;
		}
		else
			arena_unset_env(env_arena, args[i]);
		i++;
	}
	return (return_value);
}

int builtin_env(t_arena *arena, t_exec_status *status)
{
	size_t i;

	if (!arena || !arena->ptrs)
	{
		handle_exec_error(status, "env: environment not set", 1);
		return (1);
	}
	i = 0;
	while (i < arena->ptrs_in_use)
	{
		ft_putstr_fd(arena->ptrs[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}