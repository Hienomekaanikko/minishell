/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:18 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/29 17:44:31 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arena	*init_env_arena(char **envp, t_data *data)
{
	size_t	env_count;
	size_t	i;

	if (envp == NULL || *envp == NULL || **envp == '\0')
		return (init_backup_env(data));
	env_count = 0;
	while (envp[env_count])
		env_count++;
	data->env_arena = arena_init(env_count * 100 * 2, env_count + 32);
	if (!data->env_arena)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		arena_add(data, envp[i]);
		i++;
	}
	set_shell_level(data);
	if (data->mem_error)
		return (NULL);
	data->env_arena->ptrs[data->env_arena->ptrs_in_use] = NULL;
	return (data->env_arena);
}

int	arena_set_env(t_data *data, char *key, char *value)
{
	char	*env_var;

	arena_unset_env(data->env_arena, key);
	if (!data->env_arena || !key)
		return (-1);
	if (!value)
	{
		if (!arena_add(data, key))
			return (-1);
		return (0);
	}
	env_var = ft_strjoin(key, "=");
	if (!env_var)
		return (-1);
	env_var = ft_strjoin_free(env_var, value);
	if (!env_var)
		return (-1);
	if (!arena_add(data, env_var))
	{
		free(env_var);
		return (-1);
	}
	free(env_var);
	return (0);
}

int	arena_unset_env(t_arena *arena, char *key)
{
	size_t	i;
	size_t	key_len;

	if (!arena || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (i < arena->ptrs_in_use)
	{
		if (ft_strncmp(arena->ptrs[i], key, key_len) == 0 &&
			(arena->ptrs[i][key_len] == '=' || arena->ptrs[i][key_len] == '\0'))
		{
			while (i < arena->ptrs_in_use - 1)
			{
				arena->ptrs[i] = arena->ptrs[i + 1];
				i++;
			}
			arena->ptrs_in_use--;
			arena->ptrs[arena->ptrs_in_use] = NULL;
			continue;
		}
		i++;
	}
	return (0);
}

char	*arena_getenv(t_data *data, t_arena *env_arena, char *key)
{
	size_t	i;
	size_t	key_len;
	char	*env_var;
	char	*result;

	if (!env_arena || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (i < env_arena->ptrs_in_use)
	{
		env_var = env_arena->ptrs[i];
		if (ft_strncmp(env_var, key, key_len) == 0 && env_var[key_len] == '=')
		{
			result = ft_strdup(env_var + key_len + 1);
			if (set_mem_error(data, result))
				return (NULL);
			return (result);
		}
		i++;
	}
	return (NULL);
}
