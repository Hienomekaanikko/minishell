/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_append.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:04:16 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/11 10:46:08 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file builtin_export_append.c
 * @brief Handles append during export
 */

/**
 * @brief Appends export when using export += something
 * @param data The main data structure of the program
 * @param key Key that is being appended into
 * @param value Value that is being appended
 * @retval 1 (failure)
 * @retval 0 (success)
 */
int	export_append(t_data *data, const char *key, char *value)
{
	char	*old_value;
	char	*new_value;

	old_value = is_declared(data, (char *)key);
	if (data->mem_error == 1)
		return (1);
	if (!old_value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return (1);
	}
	else
	{
		new_value = ft_strjoin(old_value, value);
		free(old_value);
	}
	if (!new_value)
	{
		data->mem_error = 1;
		return (1);
	}
	arena_set_env(data, (char *)key, new_value);
	free(new_value);
	return (0);
}
