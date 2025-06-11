/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:11:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 11:40:27 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_variables(t_data *data, t_exp_tools *tools, char **value_ptr)
{
	int		i;
	char	*value;

	i = 0;
	value = *value_ptr;
	while (value[i])
	{
		if (value[i] == '$')
		{
			if (!append_pre_dollar(data, tools, value, i))
				return (0);
			if (!dollar_expansion(data, tools, value, &i))
				return (0);
			value += i;
			i = 0;
		}
		else
			i++;
	}
	*value_ptr = value;
	return (1);
}

char	*expand(t_data *data, t_exp_tools *tools, char *value)
{
	tools->result = ft_strdup("");
	if (set_mem_error(data, tools->result))
		return (NULL);
	if (!expand_variables(data, tools, &value))
		return (NULL);
	if (!append_post_dollar(data, tools, value))
		return (NULL);
	if (ft_strlen(tools->result) == 0)
		return (NULL);
	return (tools->result);
}

int	expand_exit_status(t_data *data, t_exp_tools *tools, int *i)
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

int	expand_var_name(t_data *data, t_exp_tools *tools, char *value, int *i)
{
	int	var_start;
	int	var_end;

	var_start = *i + 1;
	var_end = var_start;
	while (value[var_end]
		&& (ft_isalnum(value[var_end]) || value[var_end] == '_'))
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
	free(tools->val);
	tools->val = NULL;
	return (1);
}
