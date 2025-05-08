#include "minishell.h"

int	builtin_echo(char **args, t_exec_status *status, t_arena *env_arena)
{
	int		no_newline_flag;
	int		i;
	char	*env_value;

	no_newline_flag = 0;
	if (!args || !args[0])
	{
		ft_putstr_fd("\n", 1);
		status->exit_code = 0;
		return (0);
	}
	i = 1;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		i++;
		no_newline_flag = 1;
	}
	while(args[i])
	{
		if (args[i][0] == '$')
		{
			if (ft_strncmp(args[i], "$!", 3) == 0)
				ft_putnbr_fd(status->pid, 1);
			else if (ft_strncmp(args[i], "$?", 3) == 0)
				ft_putnbr_fd(status->exit_code, 1);
			else if ((env_value = arena_getenv(env_arena, args[i] + 1)))
				ft_putstr_fd(env_value, 1);
		}
		else
			ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (no_newline_flag == 0)
		ft_putstr_fd("\n", 1);
	status->exit_code = 0;
	return (0);
}

int	builtin_cd(char **args, t_exec_status *status, t_arena *env_arena)
{
	char	*new_pwd;
	char	*path;

	path = args[1];
	if (!path)
	{
		path = arena_getenv(env_arena, "HOME");
		if (!path)
			return (error_handler(status, "cd: HOME not set", 1));
	}
	if (chdir(path) == -1)
		return (error_handler(status, "cd: No such file or directory", 1));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error_handler(status, "cd: getcwd failed", 1));
	arena_set_env(env_arena, "OLDPWD", arena_getenv(env_arena, "PWD"));
	arena_set_env(env_arena, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_pwd(t_exec_status *status, t_arena *env_arena)
{
	char	*pwd;
	
	pwd = arena_getenv(env_arena, "PWD");
	if (pwd)
	{
		ft_putstr_fd(pwd, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error_handler(status, "pwd: getcwd failed", 1));
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
				return (error_handler(status, "export: Memory error", 1));
			}
			*equals = '=';
		}
		if (!is_valid_env_name(key))
			return (error_handler(status, "export: invalid identifier", 1));
		i++;
	}
	return (0);
}

int	builtin_unset(t_arena *env_arena, t_exec_status *status, char **args)
{
	int	i;

	if (!args[1])
		return (error_handler(status, "unset: not enough arguments", 1));
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			error_handler(status, "unset: invalid identifier", 1);
			return (1);
		}
		arena_unset_env(env_arena, args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_arena *arena, t_exec_status *status)
{
	size_t i;
	char	*value;

	if (!arena || !arena->ptrs)
		return (error_handler(status, "env: environment not set", 1));
	i = 0;
	while (i < arena->ptrs_in_use)
	{
		value = ft_strchr(arena->ptrs[i], '=');
		if (value && value[1])
		{
			ft_putstr_fd(arena->ptrs[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	return (0);
}

static int	is_valid_exit_arg(char *arg)
{
	if (arg[0] != '+' && arg[0] != '-' && !ft_isdigit(arg[0]))
		return (0);
	int i = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_ast *node, t_exec_status *status)
{
	if (!node->args[1])
	{
		status->exit_code = 0;
		return (1);
	}
	if (node->args[2])
		return (error_handler(status, "exit: too many arguments", 1));
	if (!is_valid_exit_arg(node->args[1]))
		return (error_handler(status, "exit: numeric argument required", 255));
	status->exit_code = (unsigned char)ft_atoi(node->args[1]);
	return (1);
}