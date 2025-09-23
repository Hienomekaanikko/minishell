/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:16:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 10:43:24 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file parser.c
 * @brief Parser
 */

/**
 * @brief Counts how many times the initial string must be split
 * @param s The string that is being handled
 * @retval parser_data.count (count of needed splits)
 */

static int	ft_count_splits(const char *s)
{
	t_counter	parser_data;

	ft_memset(&parser_data, 0, sizeof(t_counter));
	while (*s)
	{
		if (*s == '>' || *s == '<' || *s == '|')
		{
			skip_word(&parser_data, s);
			s += parser_data.op_len;
			continue ;
		}
		if (ft_isspace(*s))
		{
			parser_data.in_word = 0;
			s++;
			continue ;
		}
		if (!parser_data.in_word)
		{
			parser_data.in_word = 1;
			parser_data.count++;
		}
		s++;
	}
	return (parser_data.count);
}

/**
 * @brief Gets the range to make substring from
 * @param parser_data Toolkit for iteration
 * @param s The string that is being handled
 */

static void	get_range(t_parser *parser_data, const char *s)
{
	while (s[parser_data->i] && (parser_data->in_quote
			|| !(ft_isspace(s[parser_data->i]))))
	{
		if (!parser_data->in_quote && (s[parser_data->i] == '<'
				|| s[parser_data->i] == '>' || s[parser_data->i] == '|'))
			break ;
		if (!parser_data->in_quote && (s[parser_data->i] == '"'
				|| s[parser_data->i] == '\''))
		{
			parser_data->quote = s[parser_data->i];
			parser_data->in_quote = 1;
			parser_data->i++;
			continue ;
		}
		if (parser_data->in_quote && s[parser_data->i] == parser_data->quote)
		{
			parser_data->in_quote = 0;
			parser_data->i++;
			continue ;
		}
		parser_data->i++;
	}
}

/**
 * @brief Makes the splits of the initial string to split operators, quotes etc properly
 * @param data The main data structure of the program
 * @param s The string that is being handled
 * @param arr The array of strings that is being built
 * @retval NULL (failure)
 * @retval arr (success)
 */

static char	**split_the_strings(t_data *data, char const *s, char **arr)
{
	t_parser	parser_data;

	ft_memset(&parser_data, 0, sizeof(t_parser));
	while (s[parser_data.i])
	{
		while (ft_isspace(s[parser_data.i]))
			parser_data.i++;
		if (!s[parser_data.i])
			break ;
		if (s[parser_data.i] == '<' || s[parser_data.i] == '>'
			|| s[parser_data.i] == '|')
			add_operator(data, &parser_data, s);
		else
		{
			parser_data.start = parser_data.i;
			get_range(&parser_data, s);
		}
		if (parser_data.i > parser_data.start)
		{
			if (!add_substring(data, &parser_data, arr, s))
				return (NULL);
		}
	}
	arr[parser_data.y] = NULL;
	return (arr);
}

/**
 * @brief Wrapper to call all the parser operations from
 * @param data The main data structure of the program
 * @param s The string that is being handled
 * @retval NULL (failure)
 * @retval result (success)
 */

char	**parser(t_data *data, char const *s)
{
	char		**arr;
	int			amount_of_strings;
	char		**result;

	if (ft_is_only_space(s))
		return (NULL);
	amount_of_strings = ft_count_splits(s);
	arr = ft_calloc(amount_of_strings + 1, sizeof(char *));
	if (arr == NULL)
	{
		data->mem_error = 1;
		return (NULL);
	}
	result = split_the_strings(data, s, arr);
	if (!result && !data->syntax_err)
	{
		data->mem_error = 1;
		return (NULL);
	}
	if (data->syntax_err)
	{
		ft_free_split(arr);
		return (NULL);
	}
	return (result);
}
