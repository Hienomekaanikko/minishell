#include "minishell.h"

int	builtin_echo(char **args, t_exec_status *status, t_arena *env_arena)
{
	int		no_newline_flag;
	int		i;
	// /char	*env_value;

	(void)env_arena;
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
		// if (args[i][0] == '$')
		// {
		// 	if (ft_strncmp(args[i], "$!", 3) == 0)
		// 		ft_putnbr_fd(status->pid, 1);
		// 	else if (ft_strncmp(args[i], "$?", 3) == 0)
		// 		ft_putnbr_fd(status->exit_code, 1);
		// }
		// 	else if ((env_value = arena_getenv(env_arena, args[i] + 1)))
		// 		ft_putstr_fd(env_value, 1);
		// }
		// else
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

	if (args [2])
		return (error_handler(status, "too many arguments", 1));
	path = args[1];
	if (!path)
	{
		path = arena_getenv(env_arena, "HOME");
		if (!path)
			return (error_handler(status, "not set", 1));
	}
	if (chdir(path) == -1)
		return (error_handler(status, strerror(errno), 1));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error_handler(status, strerror(errno), 1));
	arena_set_env(env_arena, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}
//Do we need to implement the oldpwd thing? cd -
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
		return (error_handler(status, strerror(errno), 1));
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
	char	*key;

	if (!args[1])
		return (builtin_env(env_arena, status));
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			key = ft_strndup(args[i], ft_strchr(args[i], '=') - args[i]);
			if (!key)
				return (error_handler(status, strerror(errno), 1));
		}
		else
			key = args[i];
		if (!is_valid_env_name(key))
		{
			if (ft_strchr(args[i], '='))
				free(key);
			return (error_handler(status, "not a valid identifier", 1));
		}
		if (ft_strchr(args[i], '=') && arena_set_env(env_arena, key, ft_strchr(args[i], '=') + 1) == -1)
		{
			free(key);
			return (error_handler(status, strerror(errno), 1));
		}
		if (ft_strchr(args[i], '='))
			free(key);
		i++;
	}
	return (0);
}

int	builtin_unset(t_arena *env_arena, t_exec_status *status, char **args)
{
	int	i;

	// if (!args[1])
	// 	return (error_handler(status, "unset", ERR_NOT_ENOUGH_ARGS));
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			error_handler(status, "not a valid identifier", 1);
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
		return (error_handler(status, "env not set", 1));
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
		return (error_handler(status, "too many arguments",1));
	if (!is_valid_exit_arg(node->args[1]))
		return (error_handler(status, "numeric argument required",  2));
	status->exit_code = (unsigned char)ft_atoi(node->args[1]);
	return (1);
}
