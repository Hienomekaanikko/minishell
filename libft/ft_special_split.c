/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_special_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:15:16 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 11:38:13 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	ft_count_splits(const char *s, char c)
{
	int	count = 0;
	int	in_word = 0;

	while (*s)
	{
		if (*s == '>' || *s == '<' || *s == '|')
		{
			char op = *s;
			int op_len = 0;
			while (*s == op && op_len < 2)
			{
				op_len++;
				s++;
			}
			count++;
			in_word = 0;
			continue;
		}
		if (*s == c)
		{
			in_word = 0;
			s++;
			continue;
		}
		if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	**free_malloc(char **array_of_strings, int y)
{
	while (y >= 0)
	{
		free(array_of_strings[y]);
		y--;
	}
	free(array_of_strings);
	return (NULL);
}

static char **split_the_strings(char const *s, char c, char **array_of_strings)
{
	int		i;
	int		y;
	int		start;
	int		in_quote;
	int		in;
	char	quote;
	char	op;

	i = 0;
	y = 0;
	in_quote = 0;
	quote = 0;
	in = 0;
	op = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break;
		if (s[i] == '<' || s[i] == '>' || s[i] == '|')
		{
			op = s[i];
			start = i;
			while (s[i] && in < 2 && s[i] == op)
			{
				i++;
				in++;
			}
			in = 0;
		}
		else
		{
			start = i;
			while (s[i] && (in_quote || s[i] != c))
			{
				if (!in_quote && (s[i] == '"' || s[i] == '\''))
				{
					quote = s[i];
					in_quote = 1;
					i++;
					continue;
				}
				if (in_quote && s[i] == quote)
				{
					in_quote = 0;
					i++;
					continue;
				}
				i++;
			}
		}
		if (i > start)
		{
			array_of_strings[y] = ft_substr(s, start, i - start);
			if (!array_of_strings[y])
				return (free_malloc(array_of_strings, y));
			y++;
		}
	}
	array_of_strings[y] = NULL;
	return (array_of_strings);
}

static int	is_only_space(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			return (0);
		i++;
	}
	return (1);
}

char	**ft_special_split(char const *s, char c)
{
	char	**array_of_strings;
	int		amount_of_strings;
	char	**result;

	if (!s)
		return (NULL);
	if (is_only_space(s))
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
