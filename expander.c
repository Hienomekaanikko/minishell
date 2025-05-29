/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:11:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/29 10:42:15 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand(t_data *data, t_exp_tools *tools, char *value)
{
	int	i;

	i = 0;
	tools->result = ft_strdup("");
	if (set_mem_error(data, tools->result))
		return (NULL);
	while (value[i])
	{
		if (value[i] == '$')
		{
			if (!before_dollar(data, tools, value, i))
				return (NULL);
			if (!dollar(data, tools, value, &i))
				return (NULL);
			value += i;
			i = 0;
		}
		else
			i++;
	}
	if (*value)
	{
		if (!after_dollar(data, tools, value))
			return (NULL);
	}
	if (ft_strlen(tools->result) == 0)
		return (NULL);
	return (tools->result);
}

int	exit_status(t_data *data, t_exp_tools *tools, int *i)
{
	tools->val = ft_itoa(data->status.exit_code);
	if (set_mem_error(data, tools->val))
		return (0);
	*i += 2;
	tools->result = ft_strjoin_free(tools->result, tools->val);
	if (set_mem_error(data, tools->result))
		return (0);
	free(tools->val);
	tools->val = NULL;
	return (1);
}

int	variable(t_data *data, t_exp_tools *tools, char *value, int *i)
{
	int	var_start;
	int	var_end;

	var_start = *i + 1;
	var_end = var_start;
	while (value[var_end] && (ft_isalnum(value[var_end]) || value[var_end] == '_'))
		var_end++;
	tools->var = ft_substr(value, var_start, var_end - var_start);
	if (set_mem_error(data, tools->var))
		return (0);
	tools->val = is_declared(data, tools->var);
	free(tools->var);
	tools->var = NULL;
	*i = var_end;
	if (tools->val)
	{
		tools->result = ft_strjoin_free(tools->result, tools->val);
		if (set_mem_error(data, tools->result))
			return (0);
	}
	return (1);
}
