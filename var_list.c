/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:22 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 13:37:04 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to create a new node
t_var	*create_var(char *key, char *value)
{
	t_var *new_node;

	new_node = malloc(sizeof(t_var));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	return (new_node);
}

// Add a new node to the end of the list
void	add_var_to_list(t_exp_data *data, char *key, char *value)
{
	t_var	*current;
	t_var	*new_node;

	if (already_declared(data->var_list, key, value))
		return ;
	new_node = create_var(key, value);
	if (!new_node)
		return ;
	if (!data->var_list)
		data->var_list = new_node;
	else
	{
		current = data->var_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
}

void	add_var_declaration(t_data *data)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	j = 0;
	while(data->input[i] != '=')
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return ;
	i = 0;
	while (data->input[i] != '=')
	{
		key[i] = data->input[i];
		i++;
	}
	key[i] = '\0';
	i++;
	while (data->input[i])
	{
		i++;
		j++;
	}
	value = malloc(sizeof(char) * (j + 1));
	if (!value)
		return ;
	i = i - j;
	j = 0;
	while (data->input[i])
	{
		value[j] = data->input[i];
		i++;
		j++;
	}
	value[j] = '\0';
	add_var_to_list(data->exp, key, value);
	free(key);
	free(value);
}
