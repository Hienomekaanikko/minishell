/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:58:42 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/05 13:38:35 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*extract_key(const char *arg, t_exec_status *status)
{
	char	*key;

	if (ft_strchr(arg, '='))
	{
		key = ft_strndup(arg, ft_strchr(arg, '=') - arg);
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

int	env_val_join(t_data *data, const char *key, const char* arg)
{

	char	*value;

	value = is_declared(data, (char *)key);

		if (!value)
		{
			arena_set_env(data, (char *)key, NULL);
			if (data->mem_error == 1)
				return (0);
		}
		else
		{
			char *temp_value = value;
			value = ft_strjoin_free(temp_value, (char *) arg);
		}
	return (1);
}

static int	set_export_value(t_data *data, const char *key, const char *arg)
{
	char	*eq;
	char	*value;

	eq = ft_strchr(arg, '=');
	env_val_join(data, key, arg);
	if(eq && eq > arg && *(eq -1) == '+')
		env_val_join()
	if (eq)
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
