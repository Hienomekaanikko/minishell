/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:51:26 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:45:38 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file init_loop.c
 * @brief Cleans up after execution to be ready for next commandline
 */

/**
 * @brief Resets the parser tools
 * @param data The main data stucture for the program
 */
static void	reset_parser(t_data *data)
{
	if (data->temp_array)
	{
		ft_free_split(data->temp_array);
		data->temp_array = NULL;
	}
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->root)
	{
		free_ast(data->root);
		data->root = NULL;
	}
	*data->lexed_list = NULL;
}

/**
 * @brief Closes and resets all extra file descriptors
 * @param data The main data stucture for the program
 */
void	close_all_fds(t_data *data)
{
	if (data->status.infile != -1)
	{
		close(data->status.infile);
		data->status.infile = -1;
	}
	if (data->status.outfile != -1)
	{
		close(data->status.outfile);
		data->status.outfile = -1;
	}
	if (data->status.temp_fd != -1)
	{
		close(data->status.temp_fd);
		data->status.temp_fd = -1;
	}
	if (data->status.saved_stdin != -1)
	{
		close(data->status.saved_stdin);
		data->status.saved_stdin = -1;
	}
	if (data->status.saved_stdout != -1)
	{
		close(data->status.saved_stdout);
		data->status.saved_stdout = -1;
	}
}

/**
 * @brief Resets the status structure of the program back to start
 * @param data The main data stucture for the program
 */
static void	reset_exec(t_data *data)
{
	close_all_fds(data);
	data->syntax_err = 0;
	data->mem_error = 0;
	data->redir_status = 0;
	data->status.redir_fail = 0;
	data->redir_err = 0;
	data->status.here_doc_flag = 0;
	if (data->status.path)
		data->status.path = NULL;
	if (data->status.msg)
		data->status.msg = NULL;
}

/**
 * @brief Wrapper for initilization functions
 * @param data The main data stucture for the program
 */
void	init_data(t_data *data)
{
	free_exp_tools(data);
	reset_parser(data);
	reset_exec(data);
}
