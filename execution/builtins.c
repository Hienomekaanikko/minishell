/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:03 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/11 10:46:15 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (error(status, node->value, TOO_MANY, 1));
	if (!is_valid_exit_arg(node->args[1]))
		return (error(status, node->value, ONLYNUM, 2));
	status->exit_code = (unsigned char)ft_atoi(node->args[1]);
	ft_putstr_fd("exit\n", 1);
	return (1);
}
