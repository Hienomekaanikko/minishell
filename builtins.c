/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:03 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/10 13:51:51 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*new_pwd;
	char	*path;

	if (count_args(args) > 2)
		return (error(&data->status, "cd", TOO_MANY, 1));
	path = args[1];
	if (!path)
	{
		path = arena_getenv(data, data->env_arena, "HOME");
		if (data->mem_error)
			return (0);
		if (!path)
			return (error(&data->status, "cd: HOME", NOTSET, 1));
	}
		if (chdir(path) == -1)
		return (error(&data->status, "cd", NOFILE, 1));
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error(&data->status, "cd", NOFILE, 1));
	arena_set_env(data, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

int	builtin_pwd(t_data *data)
{
	char	*pwd;

	pwd = arena_getenv(data, data->env_arena, "PWD");
	if (data->mem_error)
		return (0);
	if (pwd)
	{
		ft_putstr_fd(pwd, 1);
		ft_putstr_fd("\n", 1);
		free(pwd);
		return (0);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (error(&data->status, "PWD", NO, 1));
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}

static int	is_valid_exit_arg(char *arg)
{
	int	i;

	if (arg[0] != '+' && arg[0] != '-' && !ft_isdigit(arg[0]))
		return (0);
	i = 1;
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
		return (error(status, node->cmd, TOO_MANY, 1));
	if (!is_valid_exit_arg(node->args[1]))
		return (error(status, node->cmd, ONLYNUM, 2));
	status->exit_code = (unsigned char)ft_atoi(node->args[1]);
	ft_putstr_fd("exit\n", 1);
	return (1);
}
