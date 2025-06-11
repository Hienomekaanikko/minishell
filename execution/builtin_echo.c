/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:36:26 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:46:06 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_flag(const char *arg)
{
	int	j;

	j = 1;
	if (arg[0] != '-')
		return (0);
	while (arg[j] != 'n')
	{
		j++;
		if (arg[j] != 'n')
			return (0);
	}
	return (1);
}

int	echo_exit(t_exec_status *status, int no_newline_flag)
{
	status->exit_code = 0;
	if (no_newline_flag == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	builtin_echo(char **args, t_exec_status *status)
{
	int	no_newline_flag;
	int	i;

	no_newline_flag = 0;
	if (!args || !args[0])
		return (echo_exit(status, no_newline_flag));
	i = 1;
	while (args[i] && is_n_flag(args[i]))
	{
		no_newline_flag = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	return (echo_exit(status, no_newline_flag));
}
