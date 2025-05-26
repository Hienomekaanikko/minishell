/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:16:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 14:15:30 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_splits(const char *s, char c)
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
		if (*s == c)
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

static void	get_range(t_parser *parser_data, const char *s, char c)
{
	while (s[parser_data->i] && (parser_data->in_quote
			|| s[parser_data->i] != c))
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

static char	**split_the_strings(char const *s, char c, char **array_of_strings)
{
	t_parser	parser_data;

	ft_memset(&parser_data, 0, sizeof(t_parser));
	while (s[parser_data.i])
	{
		while (s[parser_data.i] == c)
			parser_data.i++;
		if (!s[parser_data.i])
			break ;
		if (s[parser_data.i] == '<' || s[parser_data.i] == '>'
			|| s[parser_data.i] == '|')
			add_operator(&parser_data, s);
		else
		{
			parser_data.start = parser_data.i;
			get_range(&parser_data, s, c);
		}
		if (parser_data.i > parser_data.start)
		{
			if (!make_substring(&parser_data, s, array_of_strings))
				return (NULL);
		}
	}
	array_of_strings[parser_data.y] = NULL;
	return (array_of_strings);
}

char	**parser(char const *s, char c)
{
	char		**array_of_strings;
	int			amount_of_strings;
	char		**result;

	if (!s)
		return (NULL);
	if (ft_is_only_space(s))
		return (NULL);
	amount_of_strings = ft_count_splits(s, c);
	array_of_strings = malloc((amount_of_strings + 1) * sizeof(char *));
	if (array_of_strings == NULL)
		return (NULL);
	result = split_the_strings(s, c, array_of_strings);
	if (!result)
		return (NULL);
	return (result);
}
