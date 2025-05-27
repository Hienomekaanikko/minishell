/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:22 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/27 10:02:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to create a new node
t_var	*create_var(char *key, char *value)
{
	t_var	*new_node;

	new_node = malloc(sizeof(t_var));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

// Add a new node to the end of the list
int	add_var_to_list(t_exp_data *data, char *key, char *value)
{
	t_var	*current;
	t_var	*new_node;

	if (already_declared(data->var_list, key, value))
		return (1);
	new_node = create_var(key, value);
	if (!new_node)
		return (0);
	if (!data->var_list)
		data->var_list = new_node;
	else
	{
		current = data->var_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (1);
}

void	add_var_declaration(t_data *data)
{
	int		i;
	int		start;
	char	*key;
	char	*value;

	i = 0;
	while (data->input[i] != '=')
		i++;
	key = ft_substr(data->input, 0, i);
	if (set_mem_error(data, key))
		return ;
	i++;
	start = i;
	while (data->input[i])
		i++;
	if (start == i)
	{
		free(key);
		return ;
	}
	value = ft_substr(data->input, start, i - start);
	if (set_mem_error(data, value))
	{
		free(key);
		return ;
	}
	else if (!add_var_to_list(data->exp, key, value))
		data->mem_error = 1;
	free(key);
	free(value);
}
