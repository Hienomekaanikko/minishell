/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:41:30 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:45:56 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file expansion_utils.c
 * @brief Expander helper functions
 */

/**
 * @brief Frees toolkit which holds for example the expanded string as result
 * @param data The main data structure of the program
 */
void	free_exp_tools(t_data *data)
{
	if (data->tools->result)
		free(data->tools->result);
	if (data->tools->var)
		free(data->tools->var);
	if (data->tools->val)
		free(data->tools->val);
	ft_memset(data->tools, 0, sizeof(t_exp_tools));
}

/**
 * @brief Removes a key that was not found
 * @param data The main data structure of the program
 * @param current Current position in the lexed linked list
 * @param prev Previous position in the lexed linked list
 * @retval next (skips onto next node in the lexed linked list)
 */
t_lexer	*remove_key_not_found(t_data *data, t_lexer *current, t_lexer *prev)
{
	t_lexer	*temp;
	t_lexer	*next;

	if (!current)
		return (NULL);
	temp = current;
	next = current->next;
	if (prev)
		prev->next = next;
	else
		*data->lexed_list = next;
	free(temp->value);
	free(temp);
	return (next);
}

/**
 * @brief Counts the amount of $ inside a string to see if there are expandables
 * @param curr  Current position in the lexed linked list
 * @retval dollars (amount of dollars)
 */
int	count_dollars(t_lexer *curr)
{
	t_lexer	*temp;
	int		i;
	int		dollars;

	temp = curr;
	i = 0;
	dollars = 0;
	while (temp)
	{
		i = 0;
		while (temp->value[i])
		{
			if (temp->value[i] == '$')
				dollars++;
			i++;
		}
		temp = temp->next;
	}
	return (dollars);
}

/**
 * @brief Checks is a key that was extracted exists in environment or in local variables
 * @param data The main data structure of the program
 * @param extracted_key The key that has been extracted
 * @retval NULL (Not declared)
 * @retval fetched_value (Value was declared and here it is)
 */
char	*is_declared(t_data *data, char *extracted_key)
{
	t_var	*temp;
	char	*fetched_value;

	if (data->mem_error || !extracted_key)
		return (NULL);
	fetched_value = NULL;
	fetched_value = arena_getenv(data, data->env_arena, extracted_key);
	if (fetched_value)
		return (fetched_value);
	temp = data->exp->var_list;
	while (temp)
	{
		if (ft_strncmp(extracted_key, temp->key, ft_strlen(extracted_key)) == 0)
		{
			fetched_value = ft_strdup(temp->value);
			if (set_mem_error(data, fetched_value))
				return (NULL);
			return (fetched_value);
		}
		temp = temp->next;
	}
	return (NULL);
}
