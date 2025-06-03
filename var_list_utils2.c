/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:51:14 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 16:30:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_variable(t_data *data, char *key, char *value)
{
	if (arena_has_key(data->env_arena, key))
		arena_set_env(data, key, value);
	if (!declared(data, data->exp->var_list, key, value))
	{
		if (!data->mem_error)
		{
			if (!add_var_to_list(data, data->exp, key, value))
				data->mem_error = 1;
		}
	}
}

void	unset_local(t_var **head, char *key)
{
	t_var	*current;
	t_var	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			if (current->key)
				free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
	return ;
}
