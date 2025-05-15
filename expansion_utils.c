/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:41:30 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/15 15:16:01 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	refresh_value(t_lexer *current, char *expanded_value, t_lexer *prev)
{
	(void)prev; //MB. Not needed??
	free(current->value);
	current->value = ft_strdup(expanded_value);
	if (!current->value)
		return ;
	free(expanded_value);
	prev = current;
	current = current->next;
}

t_lexer *remove_key_not_found(t_data *data, t_lexer *current, t_lexer *prev)
{
	t_lexer *temp;
	t_lexer *next;

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

char	*is_declared(t_data *data, char *extracted_key, t_arena *env_arena)
{
	t_var	*temp;
	char	*fetched_value;

	fetched_value = NULL;
	fetched_value = arena_getenv(env_arena, extracted_key);
	if (fetched_value)
		return (fetched_value);
	temp = data->exp->var_list;
	while (temp)
	{
		if (ft_strncmp(extracted_key, temp->key, ft_strlen(extracted_key)) == 0)
		{
			fetched_value = ft_strdup(temp->value);
			if (!fetched_value)
				return (NULL);
			return (fetched_value);
		}
		temp = temp->next;
	}
	return (NULL);
}
