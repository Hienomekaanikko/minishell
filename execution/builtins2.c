/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:02:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:21:10 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file builtins2.c
 * @brief Built-in handlers helper functions
 */

/**
 * @brief Checks if valid value to look for from env
 * @param name Value that is being looked
 * @retval 0 (false)
 * @retval 1 (true)
 */
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

/**
 * @brief Unsets a variable from the env and locals
 * @param data The main data structure of the program
 * @param args The array of strings that starts with UNSET
 * @retval 1 (fail)
 * @retval 0 (success)
 */
int	builtin_unset(t_data *data, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (!is_valid_env_name(args[i]))
		{
			error(&data->status, "unset", NOT_VALID, 1);
			return (1);
		}
		arena_unset_env(data->env_arena, args[i]);
		i++;
	}
	unset_local(&data->exp->var_list, args[1]);
	return (0);
}

/**
 * @brief Handles the 'env' command
 * @param data The main data structure of the program
 * @retval 1 (fail)
 * @retval 0 (success)
 */
int	builtin_env(t_data *data)
{
	size_t	i;
	char	*value;

	if (!data->env_arena || !data->env_arena->ptrs)
		return (error(&data->status, NULL, NOENV, 1));
	i = 0;
	while (i < data->env_arena->ptrs_in_use)
	{
		value = ft_strchr(data->env_arena->ptrs[i], '=');
		if (value)
		{
			ft_putstr_fd(data->env_arena->ptrs[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	return (0);
}
