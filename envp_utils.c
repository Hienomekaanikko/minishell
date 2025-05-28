/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:04:38 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 15:05:55 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_shell_level(t_data *data)
{
	int		level;
	char	*old_level;
	char	*new_level;

	old_level = arena_getenv(data->env_arena, "SHLVL");
	level = ft_atoi(old_level);
	level++;
	new_level = ft_itoa(level);
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
	arena_set_env(data, "PATH", SECURE_PATH);
	arena_set_env(data, "LOGNAME", "USER");
	return (data->env_arena);
}
