/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:22 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 14:02:16 by msuokas          ###   ########.fr       */
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

static int	check_input(char *input, char **key, int *i, int *start)
{
	*i = 0;
	while (input[*i] && input[*i] != '=')
		(*i)++;
	if (input[*i] != '=')
		return (0);
	*key = ft_substr(input, 0, *i);
	if (!*key)
		return (-1);
	(*i)++;
	*start = *i;
	while (input[*i])
		(*i)++;
	if (*start == *i)
	{
		free(*key);
		*key = NULL;
		return (0);
	}
	return (1);
}

void	add_var_declaration(t_data *data)
{
	int		i;
	int		start;
	int		status;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	status = check_input(data->input, &key, &i, &start);
	if (status == -1)
		set_mem_error(data, key);
	else if (status == 0)
		return ;
	else
	{
		value = ft_substr(data->input, start, i - start);
		if (!value || !add_var_to_list(data->exp, key, value))
			data->mem_error = 1;
	}
	free(key);
	free(value);
}
