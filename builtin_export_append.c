/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_append.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 14:04:16 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/10 10:35:53 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if(!new_value)
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
