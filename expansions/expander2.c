/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:17:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 11:38:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_post_dollar(t_data *data, t_exp_tools *tools, char *value)
{
	tools->result = ft_strjoin_free(tools->result, value);
	if (set_mem_error(data, tools->result))
		return (0);
	return (1);
}

int	dollar_literal(t_data *data, t_exp_tools *tools, int *i)
{
	tools->result = ft_strjoin_free(tools->result, "$");
	if (set_mem_error(data, tools->result))
		return (0);
	(*i)++;
	return (tools->result != NULL);
}

int	append_pre_dollar(t_data *data, t_exp_tools *tools, char *value, int i)
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

int	dollar_expansion(t_data *data, t_exp_tools *tools, char *value, int *i)
{
	int	start;

	start = *i + 1;
	if (value[start] == '?')
		return (expand_exit_status(data, tools, i));
	else if (!value[start] || (!ft_isalnum(value[start])))
		return (dollar_literal(data, tools, i));
	else
		return (expand_var_name(data, tools, value, i));
}
