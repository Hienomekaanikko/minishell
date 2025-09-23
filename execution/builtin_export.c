/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:58:42 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:46:13 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file builtin_export.c
 * @brief Handles export
 */

/**
 * @brief Export handling without arguments
 * @param env_arena Pointer to the environment
 * @retval 0 (success)
 */
static int	export_no_args(t_arena *env_arena)
{
	size_t	i;
	char	*eq;

	i = 0;
	while (i < env_arena->ptrs_in_use)
	{
		eq = ft_strchr(env_arena->ptrs[i], '=');
		ft_putstr_fd("declare -x ", 1);
		if (eq)
		{
			write(1, env_arena->ptrs[i], eq - env_arena->ptrs[i]);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(eq + 1, 1);
			ft_putstr_fd("\"\n", 1);
		}
		else
		{
			ft_putstr_fd(env_arena->ptrs[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Extracts key from the string starting with export
 * @param arg The string that is being extracted from
 * @param status Pointer to the program status structure
 * @retval NULL (failure)
 * @retval key (success)
 */
static char	*extract_key(const char *arg, t_exec_status *status)
{
	char	*key;
	char	*eq;
	char	*plus;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		plus = NULL;
		if (eq > arg && *(eq - 1) == '+')
			plus = (char *)(eq - 1);
		if (plus)
			key = ft_strndup(arg, plus - arg);
		else
			key = ft_strndup(arg, eq - arg);
		if (!key)
		{
			error(status, "export", MALLOC, 1);
			return (NULL);
		}
	}
	else
		key = (char *)arg;
	return (key);
}

/**
 * @brief Separates key from the string that begins with export
 * @param arg The string that is being handled
 * @param status Pointer to the program status structure
 * @param out_key Pointer to the location where the extracted key will be saved into
 * @retval 1 (failure)
 * @retval 0 (success)
 */
static int	get_key(char *arg, t_exec_status *status, char **out_key)
{
	char	*key;

	key = extract_key(arg, status);
	if (!key)
		return (1);
	if (!is_valid_env_name(key))
	{
		if (ft_strchr(arg, '='))
			free(key);
		error(status, "export", NOT_VALID, 1);
		return (1);
	}
	*out_key = key;
	return (0);
}

/**
 * @brief Takes the value from export and pairs it up with the correct key
 * @param data The main data structure of the program
 * @param key Key that is being paired to
 * @param arg The string from where value gets extracted from
 * @retval 0
 */
static int	set_export_value(t_data *data, const char *key, const char *arg)
{
	char	*eq;
	char	*value;

	eq = ft_strchr(arg, '=');
	if (eq && eq > arg && *(eq - 1) == '+')
		return (export_append(data, key, eq + 1));
	else if (eq)
		arena_set_env(data, (char *)key, eq + 1);
	else
	{
		value = is_declared(data, (char *)key);
		if (data->mem_error == 1)
			return (0);
		if (value)
		{
			arena_set_env(data, (char *)key, value);
			free(value);
		}
		else
			arena_set_env(data, (char *)key, NULL);
		if (data->mem_error == 1)
			return (0);
	}
	return (0);
}

/**
 * @brief Handles the builtin export function
 * @param data The main data structure of the program
 * @param args The array of strings that begins with export and needs to be managed
 * @retval 1 (success)
 * @retval 0 (failure)
 */
int	builtin_export(t_data *data, char **args)
{
	size_t	i;
	char	*key;

	if (!args[1])
		return (export_no_args(data->env_arena));
	i = 1;
	while (args[i])
	{
		if (get_key(args[i], &data->status, &key))
			return (1);
		if (set_export_value(data, key, args[i]))
		{
			if (ft_strchr(args[i], '='))
			{
				free(key);
				return (1);
			}
		}
		if (ft_strchr(args[i], '='))
			free(key);
		i++;
	}
	return (0);
}
