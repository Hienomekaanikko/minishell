/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 13:00:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_operator(t_data *data, t_parser *pars, const char *s)
{
	pars->op = s[pars->i];
	pars->start = pars->i;
	while (s[pars->i] && pars->in < 2 && s[pars->i] == pars->op)
	{
		pars->i++;
		pars->in++;
	}
	if (pars->op == '|' && pars->in == 2)
		data->syntax_err = 2;
	pars->in = 0;
}

int	make_substring(t_parser *data, char const *s, char **array_of_strings)
{
	int	i;
	int	start;

	i = data->i;
	start = data->start;
	array_of_strings[data->y] = ft_substr(s, start, i - start);
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

