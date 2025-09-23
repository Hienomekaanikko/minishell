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

/**
 * @file expander2.c
 * @brief Expander helper functions
 */

/**
 * @brief Appends characters after $ (for example if multi-word string)
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteartion
 * @param value Value that is being appended
 * @retval 0 (Failure)
 * @retval 1 (Success, value updated into tools->result)
 */
int	append_post_dollar(t_data *data, t_exp_tools *tools, char *value)
{
	tools->result = ft_strjoin_free(tools->result, value);
	if (set_mem_error(data, tools->result))
		return (0);
	return (1);
}

/**
 * @brief Handles $ that is not connected to expansions (appends it to the result)
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteartion
 * @param i Iterator
 * @retval 0 (Failure)
 * @retval 1 (Success, value updated into tools->result)
 */
int	dollar_literal(t_data *data, t_exp_tools *tools, int *i)
{
	tools->result = ft_strjoin_free(tools->result, "$");
	if (set_mem_error(data, tools->result))
		return (0);
	(*i)++;
	return (tools->result != NULL);
}

/**
 * @brief Handles characters before $ and includes it into tools->result in case of a multi-word string
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteartion
 * @param value Value that is being scanned
 * @param i Iterator
 * @retval 0 (Failure)
 * @retval 1 (Success, value updated into tools->result)
 */
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

/**
 * @brief Handles expansion and for example $? which gives the exit status of the last command
 * @param data The main data structure of the program
 * @param tools Toolkit for the iteartion
 * @param value Value that is being scanned
 * @param i Iterator
 * @retval (Status of expansion functions that are being called)
 */
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
