/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:22 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/02 15:57:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to create a new node
t_var	*create_var(t_data *data, char *key, char *value)
{
	t_var	*new_node;

	(void)value;
	new_node = malloc(sizeof(t_var));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (set_mem_error(data, new_node->key))
		return (NULL);
	new_node->value = ft_strdup(value);
	if (set_mem_error(data, new_node->value))
	{
		free(new_node->key);
		new_node->key = NULL;
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

// Add a new node to the end of the list
int	add_var_to_list(t_data *d, t_exp_data *exp, char *key, char *value)
{
	t_var	*current;
	t_var	*new_node;

	if (declared(d, exp->var_list, key, value))
		return (1);
	if (d->mem_error == 1)
		return (0);
	new_node = create_var(d, key, value);
	if (!new_node)
		return (0);
	if (!exp->var_list)
		exp->var_list = new_node;
	else
	{
		current = exp->var_list;
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
	int		err;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	err = check_input(data->input, &key, &i, &start);
	if (err == -1)
		set_mem_error(data, key);
	else if (err == 0)
		return ;
	else
	{
		value = ft_substr(data->input, start, i - start);
		if (set_mem_error(data, value))
			return ;
		else
			set_variable(data, key, value);
	}
	if (key)
		free(key);
	if (value)
		free(value);
}
