/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:06:07 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:46:53 by msuokas          ###   ########.fr       */
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

int	is_var_declaration(char *str)
{
	int		i;

	i = 0;
	if (!str || ft_strlen(str) == 0)
		return (0);
	while (str[i] && ft_isalnum(str[i]))
		i++;
	if (str[i] != '=')
		return (0);
	i++;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	declared(t_data *data, t_var *start, char *key, char *value)
{
	t_var	*temp;
	int		key_len;

	temp = start;
	key_len = ft_strlen(key);
	if (key_len == 0)
		return (0);
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, key_len) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			if (set_mem_error(data, temp->value))
				return (0);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
