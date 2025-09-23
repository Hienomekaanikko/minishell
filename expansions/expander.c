/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:11:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 16:03:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file expander.c
 * @brief Functions that handle the expansion itself
 */

/**
 * @brief Scans a string for $ symbols. When found, handles text before $, expands the variable after it, and rebuilds the string. Updates *value_ptr to the new expanded string.
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteartion
 * @param value_ptr Pointer to the variable which holds the expanded value
 * @retval 0 (Failure)
 * @retval 1 (Success)
 */
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

/**
 * @brief Main entry point for expansion. Initializes result, calls expand_variables, and appends remaining text after last $. Returns the fully expanded string or NULL if expansion failed.
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteartion
 * @param value Value that is being scanned
 * @retval NULL (Failure)
 * @retval tools->result (Refactored string)
 */
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

/**
 * @brief Expands $? into the shell’s last exit code. Converts exit_code to a string, appends it to the result, and moves the index past $?.
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteration
 * @param i Iterator position
 * @retval 0 (Failure)
 * @retval 1 (Success)
 */
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

/**
 * @brief Expands $something. Extracts the variable name, looks it up in the environment, and if found, appends its value to the result. Advances the index past the variable name.
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteration
 * @param value The value being scanned
 * @param i Iterator position
 * @retval 0 (Failure)
 * @retval 1 (Success)
 */
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
