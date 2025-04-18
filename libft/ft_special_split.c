/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_special_split.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:15:16 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/18 15:47:36 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_splits(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
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

static char	**split_the_strings(char const *s, char c, char **array_of_strings)
{
	int		i;
	int		j;
	int		y;
	int		in_quote;
	char	quote;

	i = 0;
	y = 0;
	in_quote = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (!s[i])
			break ;
		if (s[i] == '"' || s[i] == '\'')
		{
			quote = s[i];
			in_quote = 1;
		}
		j = i;
		if (in_quote)
		{
			i++;
			while (s[i])
			{
				if (s[i] == quote)
				{
					i++;
					break ;
				}
				i++;
			}
			if (ft_isalnum(s[i]))
			{
				in_quote = 0;
				continue;
			}
			in_quote = 0;
		}
		else
		{
			while (s[i] && s[i] != c)
				i++;
		}
		array_of_strings[y] = ft_substr(s, j, i - j);
		if (!array_of_strings[y])
		{
			free_malloc(array_of_strings, y);
			return (NULL);
		}
		y++;
	}
	array_of_strings[y] = NULL;
	return (array_of_strings);
}

char	**ft_special_split(char const *s, char c)
{
	char	**array_of_strings;
	int		amount_of_strings;
	char	**result;

	if (!s)
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
