/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:49:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 13:22:25 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	allocate_structs(t_data *data)
{
	data->exp = malloc(sizeof(t_exp_data));
	if (!data->exp)
		return (0);
	ft_memset(data->exp, 0, sizeof(t_exp_data));
	data->lexed_list = malloc(sizeof(t_lexer *));
	if (!data->lexed_list)
	{
		free(data->exp);
		return (0);
	}
	data->tools = malloc(sizeof(t_exp_tools));
	if (!data->tools)
	{
		free(data->exp);
		free_lexed_list(*data->lexed_list);
		return (0);
	}
	ft_memset(data->tools, 0, (sizeof(t_exp_tools)));
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
	data->status.path = NULL;
	data->status.msg = NULL;
}

int	init_base(t_data *data, int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	data->root = NULL;
	data->input = NULL;
	data->tools = NULL;
	data->root = NULL;
	data->temp_array = NULL;
	data->mem_error = 0;
	data->redir_err = 0;
	data->rl_linecount = 0;
	if (!allocate_structs(data))
		return (0);
	init_exec_status(data);
	if (!init_env_arena(envp, data))
		return (0);
	return (1);
}
