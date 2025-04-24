/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:22 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 14:20:10 by msuokas          ###   ########.fr       */
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
	int		start;
	char	*key;
	char	*value;

	i = 0;
	j = 0;
	while(data->input[i] != '=')
		i++;
	key = ft_substr(data->input, 0, i);
	i++;
	start = i;
	while (data->input[i])
		i++;
	value = ft_substr(data->input, start, i - start);
	add_var_to_list(data->exp, key, value);
	free(key);
	free(value);
}
