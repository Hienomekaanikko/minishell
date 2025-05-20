/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:03 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/20 10:16:30 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_data *data)
{
	int		no_newline_flag;
	int		i;
	// /char	*env_value;

	//(void)data;
	no_newline_flag = 0;
	if (!data->root->args || !data->root->args[0])
	{
		ft_putstr_fd("\n", 1);
		data->status->exit_code = 0;
		return (0);
	}
	i = 1;
	while (data->root->args[i] && ft_strncmp(data->root->args[i], "-n", 3) == 0)
	{
		i++;
		no_newline_flag = 1;
	}
	while(data->root->args[i])
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
		ft_putstr_fd(data->root->args[i], 1);
		if (data->root->args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (no_newline_flag == 0)
		ft_putstr_fd("\n", 1);
	data->status->exit_code = 0;
	return (0);
}

int	builtin_cd(t_data *data)
{
	char	*new_pwd;
	char	*path;

	if (data->root->args[2])
		return (error_handler(data, "cd", "too many arguments", 1));
	path = data->root->args[1];
	if (!path)
	{
		path = arena_getenv(data, "HOME");
		if (!path)
			return (error_handler(data, "env", "not set", 1));
	}
	if (chdir(path) == -1)
		return (error_handler(data, "cd", strerror(errno), 1));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error_handler(data, "cd", strerror(errno), 1));
	arena_set_env(data, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}
//Do we need to implement the oldpwd thing? cd -
int	builtin_pwd(t_data *data)
{
	char	*pwd;

	pwd = arena_getenv(data, "PWD");
	if (pwd)
	{
		ft_putstr_fd(pwd, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error_handler(data, "PWD", strerror(errno), 1));
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

int	builtin_export(t_data *data)
{
	int		i;
	char	*key;

	if (!data->root->args[1])
		return (builtin_env(data));
	i = 1;
	while (data->root->args[i])
	{
		if (ft_strchr(data->root->args[i], '='))
		{
			key = ft_strndup(data->root->args[i], ft_strchr(data->root->args[i], '=') - data->root->args[i]);
			if (!key)
				return (error_handler(data, "export", strerror(errno), 1));
		}
		else
			key = data->root->args[i];
		if (!is_valid_env_name(key))
		{
			if (ft_strchr(data->root->args[i], '='))
				free(key);
			return (error_handler(data, data->root->args[0], "not a valid identifier", 1));
		}
		if (ft_strchr(data->root->args[i], '=') && arena_set_env(data, key, ft_strchr(data->root->args[i], '=') + 1) == -1)
		{
			free(key);
			return (error_handler(data, "export" ,strerror(errno), 1));
		}
		if (ft_strchr(data->root->args[i], '='))
			free(key);
		i++;
	}
	return (0);
}

int	builtin_unset(t_data *data)
{
	int	i;	

	//if (!data->root->args[1])
	//	return (error_handler(data->status, "unset", ERR_NOT_ENOUGH_ARGS));
	i = 1;
	while (data->root->args[i])
	{
		if (!is_valid_env_name(data->root->args[i]))
		{
			error_handler(data, "unset", "not a valid identifier", 1);
			return (1);
		}
		arena_unset_env(data, data->root->args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_data *data)
{
	size_t i;
	char	*value;

	if (!data->env_arena || !data->env_arena->ptrs)
		return (error_handler(data, NULL, "env not set", 1));
	i = 0;
	while (i < data->env_arena->ptrs_in_use)
	{
		value = ft_strchr(data->env_arena->ptrs[i], '=');
		if (value && value[1])
		{
			ft_putstr_fd(data->env_arena->ptrs[i], 1);
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

int	builtin_exit(t_data *data)
{
	if (!data->root->args[1])
	{
		data->status->exit_code = 0;
		return (1);
	}
	if (data->root->args[2])
		return (error_handler(data, data->root->cmd, "too many arguments",1));
	if (!is_valid_exit_arg(data->root->args[1]))
		return (error_handler(data, data->root->cmd, "numeric argument required",  2));
	data->status->exit_code = (unsigned char)ft_atoi(data->root->args[1]);
	return (1);
}
