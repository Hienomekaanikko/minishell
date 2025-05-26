/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 11:10:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_operator(t_parser *data, const char *s)
{
	data->op = s[data->i];
	data->start = data->i;
	while (s[data->i] && data->in < 2 && s[data->i] == data->op)
	{
		data->i++;
		data->in++;
	}
	data->in = 0;
}

int	make_substring(t_parser *data, char const *s, char **array_of_strings)
{
	(void)s;
	array_of_strings[data->y] = ft_substr(s, data->start, data->i - data->start);
	if (!array_of_strings[data->y])
	{
		free_malloc(array_of_strings, data->y);
		return (0);
	}
	data->y++;
	return (1);
}

char	**free_malloc(char **array_of_strings, int y)
{
	while (y >= 0)
	{
		free(array_of_strings[y]);
		y--;
	}
	free(array_of_strings);
	return (NULL);
}

void	skip_word(t_counter *data, const char *s)
{
	data->op = *s;
	data->op_len = 0;
	while (*s == data->op && data->op_len < 2)
	{
		data->op_len++;
		s++;
	}
	data->count++;
	data->in_word = 0;
}
