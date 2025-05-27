/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:51:26 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/27 13:44:07 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	reset_exec(t_data *data)
{
	close_all_fds(data);
	data->syntax_err = 0;
	data->mem_error = 0;
	data->redir_status = 0;
	data->status.redir_fail = 0;
	data->status.here_doc_flag = 0;
}

void	init_data(t_data *data)
{
	reset_parser(data);
	reset_exec(data);
}
