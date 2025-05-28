/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:41:30 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 18:40:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_lexer	*remove_key_not_found(t_data *data, t_lexer *current, t_lexer *prev)
{
	t_lexer	*temp;
	t_lexer	*next;

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

char	*is_declared(t_data *data, char *extracted_key)
{
	t_var	*temp;
	char	*fetched_value;

	if (data->mem_error || !extracted_key)
		return (NULL);
	fetched_value = NULL;
	fetched_value = arena_getenv(data->env_arena, extracted_key);
	if (fetched_value)
		return (fetched_value);
	temp = data->exp->var_list;
	while (temp)
	{
		if (ft_strncmp(extracted_key, temp->key, ft_strlen(temp->key)) == 0)
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
