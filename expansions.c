/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/27 11:31:09 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_mem_error(t_data *data, char *value)
{
	if (!value)
	{
		data->mem_error = 1;
		return (1);
	}
	return (0);
}

int	strjoin_handler(t_data *data, char **dest, char **to_append)
{
	char	*new_str;

	new_str = ft_strjoin_free(*dest, *to_append);
	if (set_mem_error(data, new_str))
		return (0);
	*dest = new_str;
	if (*to_append)
	{
		free(*to_append);
		*to_append = NULL;
	}
	return (1);
}

static int	append_substring_before_dollar(t_data *data, char *value)
{
	char	*sub;
	int		i;
	int		start;

	i = data->tools->i;
	start = data->tools->start;
	sub = ft_substr(value, start, i - start);
	if (set_mem_error(data, sub))
		return (0);
	data->tools->new_value = ft_strjoin_free(data->tools->new_value, sub);
	free(sub);
	if (set_mem_error(data, data->tools->new_value))
		return (0);
	return (1);
}

static int	expand_question_mark(t_data *data)
{
	data->tools->fetched_value = ft_itoa(data->status.exit_code);
	if (set_mem_error(data, data->tools->fetched_value))
		return (0);
	if (data->tools->key_len > 1)
	{
		data->tools->leftovers = ft_substr(data->tools->extracted_key, 1, data->tools->key_len - 1);
		if (set_mem_error(data, data->tools->leftovers))
			return (0);
	}
	return (1);
}

static int	expand_exclamation_mark(t_data *data)
{
	data->tools->fetched_value = ft_itoa((int)data->status.pid);
	if (!data->tools->fetched_value || ft_strncmp(data->tools->fetched_value, "0", 1) == 0)
	{
		free(data->tools->fetched_value);
		data->tools->fetched_value = ft_strdup("");
		if (set_mem_error(data, data->tools->fetched_value))
			return (0);
	}
	if (data->tools->key_len > 1)
	{
		data->tools->leftovers = ft_substr(data->tools->extracted_key, 1, data->tools->key_len - 1);
		if (set_mem_error(data, data->tools->leftovers))
			return (0);
	}
	return (1);
}

static int	handle_special(t_data *data)
{
	if (!data->tools->extracted_key)
		return (0);
	if (ft_strncmp(data->tools->extracted_key, "?", 1) == 0)
	{
		if (!expand_question_mark(data))
			return (0);
	}
	else if (ft_strncmp(data->tools->extracted_key, "!", 1) == 0)
	{
		if (!expand_exclamation_mark(data))
			return (0);
	}
	return (1);
}

static int	get_key_len(t_data *data, char *value)
{
	data->tools->key_len = 0;
	while (value[data->tools->i + data->tools->key_len]
		&& ft_isalnum(value[data->tools->i + data->tools->key_len]))
		data->tools->key_len++;
	if (data->tools->key_len == 0)
	{
		data->tools->new_value = ft_strjoin_free(data->tools->new_value, "$");
		if (set_mem_error(data, data->tools->new_value))
			return (0);
	}
	return (1);
}

static int	append_expanded_variable(t_data *data, char *value)
{
	if (!get_key_len(data, value))
		return (0);
	data->tools->extracted_key = ft_substr(value, data->tools->i, data->tools->key_len);
	if (set_mem_error(data, data->tools->extracted_key))
		return (0);
	if (!handle_special(data))
		return (0);
	if (data->tools->fetched_value == NULL)
		data->tools->fetched_value = is_declared(data, data->tools->extracted_key);
	if (data->tools->fetched_value && !data->mem_error)
	{
		if (!strjoin_handler(data, &data->tools->new_value, &data->tools->fetched_value))
			return (0);
		if (data->tools->leftovers)
		{
			if (!strjoin_handler(data, &data->tools->new_value, &data->tools->leftovers))
				return (0);
		}
	}
	free(data->tools->extracted_key);
	data->tools->extracted_key = NULL;
	if (data->mem_error)
		return (0);
	data->tools->i += data->tools->key_len;
	return (1);
}

void	handle_dollars(t_data *data, char *value)
{
	if (value[data->tools->i] == '$')
	{
		if (data->tools->i > data->tools->start)
		{
			if (!append_substring_before_dollar(data, value))
				return ;
		}
		data->tools->i++;
		if (!append_expanded_variable(data, value))
			return ;
		data->tools->start = data->tools->i;
	}
}

char	*expander(t_data *data, char *value)
{
	size_t	len;

	len = ft_strlen(value);
	while (data->tools->i < len && !data->mem_error)
	{
		handle_dollars(data, value);
		data->tools->i++;
	}
	if (data->tools->i > data->tools->start && !data->mem_error)
	{
		if (!append_substring_before_dollar(data, value))
			return (NULL);
	}
	if (data->tools->new_value && !data->mem_error)
	{
		if (ft_strlen(data->tools->new_value) == 0)
			return (NULL);
		return (data->tools->new_value);
	}
	return (NULL);
}


void	advance_node(t_lexer **current, t_lexer **prev)
{
	if (current && *current)
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

int	is_single_quote(t_lexer **current, t_lexer **prev)
{
	if ((*current)->value[0] == '\'')
	{
		advance_node(current, prev);
		return (1);
	}
	return (0);
}

int	expand(t_lexer **current, t_lexer **prev, char *expanded_value)
{
	if (!refresh_value(*current, expanded_value))
		return (0);
	if (ft_strchr((*current)->value, '$'))
		advance_node(current, prev);
	return (1);
}

void	clear_expander_tools(t_data *data)
{
	if (data->tools->extracted_key)
		free(data->tools->extracted_key);
	if (data->tools->fetched_value)
		free(data->tools->fetched_value);
	if (data->tools->leftovers)
		free(data->tools->leftovers);
	if (data->tools->new_value)
		free(data->tools->new_value);
	ft_memset(data->tools, 0, sizeof(t_exp_tools));
}
static int	take_action(t_data *data, char **expanded_value, t_lexer **current, t_lexer **prev)
{
	*expanded_value = expander(data, (*current)->value);
	if (*expanded_value)
	{
		if (!expand(current, prev, *expanded_value))
		{
			free(*expanded_value);
			data->mem_error = 1;
			return (0);
		}
	}
	else if (data->mem_error == 0)
		*current = remove_key_not_found(data, *current, *prev);
	else if (data->mem_error ==1)
	{
		clear_expander_tools(data);
		return (0);
	}
	return (1);
}

void	check_for_expansions(t_data *data)
{
	t_lexer	*current;
	t_lexer	*prev;
	char	*expanded_value;

	current = *data->lexed_list;
	prev = NULL;
	ft_memset(data->tools, 0, sizeof(t_exp_tools));
	while (current)
	{
		if (is_single_quote(&current, &prev))
			continue ;
		if (ft_strchr(current->value, '$'))
		{
			if (!take_action(data, &expanded_value, &current, &prev))
				return ;
		}
		else
			advance_node(&current, &prev);
		clear_expander_tools(data);
	}
}
