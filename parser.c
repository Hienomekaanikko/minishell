/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:16:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/20 17:16:09 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_count_splits(const char *s, char c)
{
	t_counter	data;

	ft_memset(&data, 0, sizeof(t_counter));
	while (*s)
	{
		if (*s == '>' || *s == '<' || *s == '|')
		{
			skip_word(&data, s);
			s += data.op_len;
			continue;
		}
		if (*s == c)
		{
			data.in_word = 0;
			s++;
			continue;
		}
		if (!data.in_word)
		{
			data.in_word = 1;
			data.count++;
		}
		s++;
	}
	return (data.count);
}

static void	get_range(t_parser *data, const char *s, char c)
{
	while (s[data->i] && (data->in_quote || s[data->i] != c))
	{
		if (!data->in_quote && (s[data->i] == '<' || s[data->i] == '>' || s[data->i] == '|'))
			break ;
		if (!data->in_quote && (s[data->i] == '"' || s[data->i] == '\''))
		{
			data->quote = s[data->i];
			data->in_quote = 1;
			data->i++;
			continue;
		}
		if (data->in_quote && s[data->i] == data->quote)
		{
			data->in_quote = 0;
			data->i++;
			continue;
		}
		data->i++;
	}
}

static char	**split_the_strings(char const *s, char c, char **array_of_strings)
{
	t_parser	data;

	ft_memset(&data, 0, sizeof(t_parser));
	while (s[data.i])
	{
		while (s[data.i] == c)
			data.i++;
		if (!s[data.i])
			break;
		if (s[data.i] == '<' || s[data.i] == '>' || s[data.i] == '|')
			add_operator(&data, s);
		else
		{
			data.start = data.i;
			get_range(&data, s, c);
		}
		if (data.in_quote)
			return (NULL);
		if (data.i > data.start)
			make_substring(&data, s, array_of_strings);
	}
	array_of_strings[data.y] = NULL;
	return (array_of_strings);
}

char	**parser(char const *s, char c)
{
	char	**array_of_strings;
	int		amount_of_strings;
	char	**result;

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
