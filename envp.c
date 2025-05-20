/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:18 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/20 12:16:33 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	set_shell_level(t_arena *env_arena, t_exec_status *status)
{
	int		level;
	char	*old_level;
	char	*new_level;

	old_level = arena_getenv(env_arena, "SHLVL");
	level = ft_atoi(old_level);
	level++;
	new_level = ft_itoa(level);
	arena_set_env(env_arena, "SHLVL", new_level, status);
	free(old_level);
	free(new_level);
}

t_arena	*init_env_arena(char **envp, t_exec_status *status)
{
	t_arena	*env_arena;
	size_t	env_count;
	size_t	i;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	env_arena = arena_init(env_count * 100 * 2, env_count + 32);
	if (!env_arena)
	{
		free(env_arena);
		return (NULL);
	}
	i = 0;
	while (i < env_count)
	{
		arena_add(env_arena, envp[i], status);
		i++;
	}
	set_shell_level(env_arena, status);
	env_arena->ptrs[env_arena->ptrs_in_use] = NULL;
	return (env_arena);
}

int	arena_set_env(t_arena *env_arena, char *key, char *value, t_exec_status *status)
{
	char	*env_var;

	arena_unset_env(env_arena, key);
	if (!env_arena || !key || !value)
		return (-1);
	env_var = ft_strjoin(key, "=");
	if (!env_var)
		return (-1);
	env_var = ft_strjoin_free(env_var, value);
	if (!env_var)
		return (-1);
	if (!arena_add(env_arena, env_var, status))
	{
		free(env_var);
		return (-1);
	}
	free(env_var);
	return (0);
}

int	arena_unset_env(t_arena *env_arena, char *key)
{
	size_t	i;
	size_t	key_len;

	if (!env_arena || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (i < env_arena->ptrs_in_use)
	{
		if (ft_strncmp(env_arena->ptrs[i], key, key_len) == 0
			&& env_arena->ptrs[i][key_len] == '=')
		{
			env_arena->ptrs[i] = env_arena->ptrs[env_arena->ptrs_in_use - 1];
			env_arena->ptrs_in_use--;
			return (0);
		}
		i++;
	}
	return (0);
}

char	*arena_getenv(t_arena *env_arena, char *key)
{
	size_t	i;
	size_t	key_len;
	char	*env_var;

	if(!env_arena || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while(i < env_arena->ptrs_in_use)
	{
		env_var = env_arena->ptrs[i];
		if (ft_strncmp(env_var, key, key_len) == 0 && env_var[key_len] == '=')
			return (ft_strdup(env_var + key_len + 1));
		i++;
	}
	return (NULL);
}
