/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 14:40:51 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file parser_utils.c
 * @brief Helper functions for the parser
 */

/**
 * @brief Adds the operator in its place
 * @param data The main data structure for the program
 * @param pars Toolkit for the parsing (iteration purposes)
 * @param s the string that is being iterated
 */

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

/**
 * @brief Creates a substring with the given data
 * @param data The main data structure for the program
 * @param s The string that is being iterated
 * @param array_of_strings Pointer to the new array of string that is being created
 * @retval 0 (failure)
 * @retval 1 (success)
 */
int	make_substring(t_parser *data, char const *s, char **array_of_strings)
{
	int	i;
	int	start;

	i = data->i;
	start = data->start;
	array_of_strings[data->y] = ft_substr(s, start, i - start);
	if (!array_of_strings[data->y])
	{
		array_of_strings[data->y] = NULL;
		ft_free_split(array_of_strings);
		return (0);
	}
	data->y++;
	array_of_strings[data->y] = NULL;
	return (1);
}

/**
 * @brief Frees the array of strings in case of malloc failure and exits safely with correct message
 * @param array_of_strings Pointer to the new array of string that is being created
 * @param y Current position in the case of failure to reverse from and clean all
 * @retval NULL
 */
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

/**
 * @brief Skips an entire word
 * @param data The main data structure for the program
 * @param s The string that is being scanned
 */
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
