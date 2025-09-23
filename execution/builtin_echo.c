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

/**
 * @file builtin_echo.c
 * @brief Command handler for "echo"
 */

/**
 * @brief Looks for -n
 * @param arg The character to look from
 * @retval 0 (not n flag)
 * @retval 1 (is n flag)
 */
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

/**
 * @brief If no arguments for echo, handle gracefully
 * @param status Pointer to the status handler of the program
 * @param no_newline_flag Permission to do new line or not.
 * @retval 0 (success)
 */
int	echo_exit(t_exec_status *status, int no_newline_flag)
{
	status->exit_code = 0;
	if (no_newline_flag == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}

/**
 * @brief Handle builtin echo
 * @param args The array of strings starting with "echo"
 * @param status Pointer to the status handler of the program
 * @retval Exit status that resulted from echo
 */
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
