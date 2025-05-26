/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:03 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/26 13:26:24 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char **args, t_data *data)
{
	char	*new_pwd;
	char	*path;

	if (args [2])
		return (error_handler(&data->status, "cd", "too many arguments", 1));
	path = args[1];
	if (!path)
	{
		path = arena_getenv(data->env_arena, "HOME");
		if (!path)
			return (error_handler(&data->status, "env", "not set", 1));
	}
	if (chdir(path) == -1)
		return (error_handler(&data->status, "cd", strerror(errno), 1));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error_handler(&data->status, "cd", strerror(errno), 1));
	arena_set_env(data->env_arena, "PWD", new_pwd, &data->status);
	free(new_pwd);
	return (0);
}
//Do we need to implement the oldpwd thing? cd -
int	builtin_pwd(t_data *data)
{
	char	*pwd;

	pwd = arena_getenv(data->env_arena, "PWD");
	if (pwd)
	{
		ft_putstr_fd(pwd, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error_handler(&data->status, "PWD", strerror(errno), 1));
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}

int	is_valid_env_name(const char *name)
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

int	builtin_unset(t_data *data, char **args)
{
	int	i;

	// if (!args[1])
	// 	return (error_handler(status, "unset", ERR_NOT_ENOUGH_ARGS));
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			error_handler(&data->status, "unset", "not a valid identifier", 1);
			return (1);
		}
		arena_unset_env(data->env_arena, args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_data *data)
{
	size_t i;
	char	*value;

	if (!data->env_arena || !data->env_arena->ptrs)
		return (error_handler(&data->status, NULL, "env not set", 1));
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

int	builtin_exit(t_ast *node, t_exec_status *status)
{
	if (!node || !node->args[1])
	{
		status->exit_code = 0;
		ft_putstr_fd("exit\n", 1);
		return (1);
	}
	if (node->args[2])
		return (error_handler(status, node->cmd, "too many arguments",1));
	if (!is_valid_exit_arg(node->args[1]))
		return (error_handler(status, node->cmd, "numeric argument required",  2));
	status->exit_code = (unsigned char)ft_atoi(node->args[1]);
	ft_putstr_fd("exit\n", 1);
	return (1);
}
