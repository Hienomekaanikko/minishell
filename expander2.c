/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:17:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 18:30:16 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	after_dollar(t_data *data, t_exp_tools *tools, char *value)
{
	tools->temp = ft_strjoin(tools->result, value);
	if (set_mem_error(data, tools->temp))
		return (0);
	free(tools->result);
	tools->result = tools->temp;
	return (1);
}

int	dollar_literal(t_exp_tools *tools, int *i)
{
	tools->result = ft_strjoin_free(tools->result, "$");
	(*i)++;
	return (tools->result != NULL);
}

int	before_dollar(t_data *data, t_exp_tools *tools, char *value, int i)
{
	tools->temp = ft_substr(value, 0, i);
	if (set_mem_error(data, tools->temp))
		return (0);
	tools->result = ft_strjoin_free(tools->result, tools->temp);
	if (set_mem_error(data, tools->result))
		return (0);
	free(tools->temp);
	tools->temp = NULL;
	return (1);
}

int	dollar(t_data *data, t_exp_tools *tools, char *value, int *i)
{
	int	start;

	start = *i + 1;
	if (value[start] == '?')
		return (exit_status(data, tools, i));
	else if (!value[start] || (!ft_isalnum(value[start]) && value[start] != '_'))
		return (dollar_literal(tools, i));
	else
		return (variable(data, tools, value, i));
}
