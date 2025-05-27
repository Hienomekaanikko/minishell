/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:49:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/27 11:27:48 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	allocate_structs(t_data *data)
{
	data->exp = malloc(sizeof(t_exp_data));
	if (!data->exp)
		return (0);
	data->exp->var_list = NULL;
	data->lexed_list = malloc(sizeof(t_lexer *));
	if (!data->lexed_list)
		return (0);
	data->tools = malloc(sizeof(t_exp_tools));
	if (!data->tools)
		return (0);
	return (1);
}

static void	init_exec_status(t_data *data)
{
	ft_memset(&data->status, 0, sizeof(t_exec_status));
	data->status.infile = -1;
	data->status.outfile = -1;
	data->status.temp_fd = -1;
	data->status.saved_stdin = -1;
	data->status.saved_stdout = -1;
}

int	init_base(t_data *data, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	data->root = NULL;
	data->input = NULL;
	data->tools = NULL;
	data->temp_array = NULL;
	data->mem_error = 0;
	data->redir_err = 0;
	if (!allocate_structs(data))
		return (0);
	init_exec_status(data);
	init_env_arena(envp, data);
	return (1);
}
