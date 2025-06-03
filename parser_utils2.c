/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 11:24:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 12:54:24 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
