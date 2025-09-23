/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_base.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 10:49:17 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/12 10:19:36 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file init_base.c
 * @brief Initializes values for the starting position
 */

/**
 * @brief Allocates memory for the main structures
 * @param data The main data stucture for the program
 */

static int	allocate_structs(t_data *data)
{
	data->exp = malloc(sizeof(t_exp_data));
	if (!data->exp)
		return (0);
	ft_memset(data->exp, 0, sizeof(t_exp_data));
	data->lexed_list = malloc(sizeof(t_lexer *));
	if (!data->lexed_list)
		return (0);
	data->tools = malloc(sizeof(t_exp_tools));
	if (!data->tools)
		return (0);
	ft_memset(data->tools, 0, (sizeof(t_exp_tools)));
	return (1);
}

/**
 * @brief Initializes the values for the program status structure
 * @param data The main data stucture for the program
 */

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

/**
 * @brief Initializes the values for the base structures of the program
 * @param data The main data stucture for the program
 * @param argc The argument count that was inputted at the program launch
 * @param argv The arguments that were given at the program launch
 * @param envp The environment variables
 * @retval 0 (malloc failure during initialization)
 * @retval 1 (success)
 */
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
	data->exp = NULL;
	data->lexed_list = NULL;
	data->rl_linecount = 0;
	init_exec_status(data);
	if (!allocate_structs(data))
		return (0);
	if (!init_env_arena(envp, data))
		return (0);
	return (1);
}
