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

/**
 * @file var_list_utils.c
 * @brief Local variable handler helper functions
 */

/**
 * @brief Sets a local variable as a key value pair
 * @param data The main data structure of the program
 * @param key Key of the key-value pair being added
 * @param value Value of the key-value pair being added
 */
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

/**
 * @brief Removes a local key-value pair
 * @param head Pointer to linked list that holds all local key-value pairs
 * @param key Key of the key-value pair being removed
 */
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

/**
 * @brief Checks if a string has been formed like a local variable declaration ("key=value")
 * @param str The string that is being checked
 * @retval 1 (is a local variable declaration)
 * @retval 0 (is not a local variable declaration)
 */
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

/**
 * @brief Looks if key-value pair has already been declared
 * @param data The main data structure of the program
 * @param start The root node of the linked list that holds the local variables
 * @param key The key which is being look for
 * @param value The value that is connected to the key
 * @retval 1 (Pair already exists)
 * @retval 0 (Pair does not exist)
 */
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
