/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:24:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 12:43:48 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file parser_utils2.c
 * @brief Another helper function for the parser
 */

/**
 * @brief Adds a substring to the array of strings that comes out of the parser
 * @param data The main data structure for the program
 * @param p Toolkit for the parsing (iteration purposes)
 * @param arr The array of strings where the substring will be added
 * @param s the string that is being iterated
 * @retval 1 (success)
 * @retval 0 (failure)
 */

int	add_substring(t_data *data, t_parser *p, char **arr, const char *s)
{
	if (p->in_quote)
	{
		data->syntax_err = 1;
		return (0);
	}
	if (!make_substring(p, s, arr))
		return (0);
	return (1);
}
