/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/21 11:46:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_substring_before_dollar(char **new_value, char *value, int start, int end)
{
	char	*sub;

	sub = ft_substr(value, start, end - start);
	*new_value = ft_strjoin(*new_value, sub);
	free(sub);
}

static void	append_expanded_variable(t_data *data, char **new_value, char *value, int *i, t_arena *env_arena, t_exec_status *exec_status)
{
	int		key_len;
	char	*extracted_key;
	char	*fetched_value;
	char	*leftovers;

	key_len = 0;
	fetched_value = NULL;
	extracted_key = NULL;
	leftovers = NULL;
	while (value[*i + key_len] && ft_isalnum(value[*i + key_len]))
		key_len++;
	if (key_len == 0)
	{
		(*new_value) = ft_strjoin_free(*new_value, "$");
		return ;
	}
	extracted_key = ft_substr(value, *i, key_len);
	if (ft_strncmp(extracted_key, "?", 1) == 0)
	{
		fetched_value = ft_itoa(exec_status->exit_code);
		if (key_len > 1)
			leftovers = ft_substr(extracted_key, 1, key_len - 1);
	}
	else if (ft_strncmp(extracted_key, "!", 1) == 0)
	{
		fetched_value = ft_itoa((int)exec_status->pid);
		if (!fetched_value || ft_strncmp(fetched_value, "0", 1) == 0)
		{
			free(fetched_value);
			fetched_value = ft_strdup("");
		}
		if (key_len > 1)
			leftovers = ft_substr(extracted_key, 1, key_len - 1);
	}
	else
		fetched_value = is_declared(data, extracted_key, env_arena);
	if (fetched_value)
	{
		*new_value = ft_strjoin(*new_value, fetched_value);
		if (leftovers)
		{
			*new_value = ft_strjoin(*new_value, leftovers);
			free(leftovers);
		}
		free(fetched_value);
	}
	free(extracted_key);
	*i += key_len;
}

char	*expander(t_data *data, char *value, t_arena *env_arena, t_exec_status *exec_status)
{
	char	*new_value;
	int		i;
	int		start;

	i = 0;
	start = 0;
	new_value = NULL;
	while (value[i])
	{
		if (value[i] == '$')
		{
			if (i > start)
				append_substring_before_dollar(&new_value, value, start, i);
			i++;
			append_expanded_variable(data, &new_value, value, &i, env_arena, exec_status);
			start = i;
		}
		else
			i++;
	}
	if (i > start)
		append_substring_before_dollar(&new_value, value, start, i);
	if (new_value)
	{
		if (ft_strlen(new_value) == 0)
		{
			free(new_value);
			return (NULL);
		}
		return (new_value);
	}
	return (NULL);
}

void	check_for_expansions(t_data *data, t_arena *env_arena, t_exec_status *exec_status)
{
	t_lexer	*current;
	t_lexer	*prev;
	char	*expanded_value;
	int		dollars;

	current = *data->lexed_list;
	prev = NULL;
	dollars = count_dollars(*data->lexed_list);
	if (!dollars)
		return ;
	while (current)
	{
		if (current->value[0] == '\'')
		{
			prev = current;
			current = current->next;
			continue ;
		}
		if (ft_strchr(current->value, '$'))
		{
			expanded_value = expander(data, current->value, env_arena, exec_status);
			if (expanded_value)
			{
				refresh_value(current, expanded_value, prev);
				if (ft_strchr(current->value, '$'))
				{
					prev = current;
					current = current->next;
				}
			}
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
