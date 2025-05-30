/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:04:38 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/29 16:20:13 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_shell_level(t_data *data)
{
	int		level;
	char	*old_level;
	char	*new_level;

	old_level = arena_getenv(data, data->env_arena, "SHLVL");
	if (data->mem_error)
		return ;
	level = ft_atoi(old_level);
	level++;
	new_level = ft_itoa(level);
	if (set_mem_error(data, new_level))
		return ;
	arena_set_env(data, "SHLVL", new_level);
	free(old_level);
	free(new_level);
}

t_arena	*init_backup_env(t_data *data)
{
	data->env_arena = arena_init(100 * 2, 32);
	if (!data->env_arena)
		return (NULL);
	arena_set_env(data, "SHLVL", "1");
	arena_set_env(data, "OLDPWD", "");
	arena_set_env(data, "PWD", getcwd(NULL, 0));
	arena_set_env(data, "HOME", "/");
	arena_set_env(data, "USER", "USER");
	arena_set_env(data, "PATH", "//bin:/usr/bin:/usr/local/bin");
	arena_set_env(data, "LOGNAME", "USER");
	return (data->env_arena);
}

int arena_has_key(t_arena *env_arena, char *key)
{
	size_t i;
	size_t key_len;

	if (!env_arena || !key)
		return (0);
	key_len = ft_strlen(key);
	i = 0;
	while (i < env_arena->ptrs_in_use)
	{
		if (ft_strncmp(env_arena->ptrs[i], key, key_len) == 0 &&
			(env_arena->ptrs[i][key_len] == '=' || env_arena->ptrs[i][key_len] == '\0'))
			return (1);
		i++;
	}
	return (0);
}
