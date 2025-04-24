/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 13:28:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_substring_before_dollar(char **new_value, char *value, int start, int end)
{
	char *sub;

	sub = ft_substr(value, start, end - start);
	*new_value = ft_strjoin(*new_value, sub);
	free(sub);
}

static void	append_expanded_variable(t_data *data, char **new_value, char *value, int *i)
{
	int		key_len;
	char	*extracted_key;
	char	*fetched_value;

	key_len = 0;
	while (value[*i + key_len] && ft_isalnum(value[*i + key_len]))
		key_len++;
	if (key_len == 0)
		return ;
	extracted_key = ft_substr(value, *i, key_len);
	fetched_value = is_declared(data, extracted_key);
	if (fetched_value)
	{
		*new_value = ft_strjoin(*new_value, fetched_value);
		free(fetched_value);
	}
	free(extracted_key);
	*i += key_len;
}

char	*expander(t_data *data, char *value)
{
	char	*new_value;
	int		i;
	int		start;

	i = 0;
	start = 0;
	new_value = ft_strdup("");
	while (value[i])
	{
		if (value[i] == '$')
		{
			if (i > start)
				append_substring_before_dollar(&new_value, value, start, i);
			i++;
			append_expanded_variable(data, &new_value, value, &i);
			start = i;
		}
		else
			i++;
	}
	if (i > start)
		append_substring_before_dollar(&new_value, value, start, i);
	if (ft_strlen(new_value) == 0)
	{
		free(new_value);
		return NULL;
	}
	return (new_value);
}

void	check_for_expansions(t_data *data)
{
	t_lexer	*current;
	t_lexer	*prev;
	char	*expanded_value;
	int		dollars;

	current = *data->lexed_list;
	prev = NULL;
	dollars = count_dollars(*data->lexed_list);
	data->exp->extracted_keys = malloc(sizeof(char*) * (dollars + 1));
	while (current)
	{
		if (ft_strchr(current->value, '$'))
		{
			expanded_value = expander(data, current->value);
			if (expanded_value)
				refresh_value(current, expanded_value, prev);
			else
				current = remove_key_not_found(data, current, prev);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
