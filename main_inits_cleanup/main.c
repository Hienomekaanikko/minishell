/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/18 12:02:31 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file main.c
 * @brief Entry point for the program
 */

/**
 * @brief Processes the input string
 * @param data The main data stucture for the program
 */
static int	process_input(t_data *data)
{
	data->input = readline("minishell$: ");
	if (data->input == NULL)
		return (0);
	if (ft_strlen(data->input) > 0)
		add_history(data->input);
	data->rl_linecount++;
	if (is_var_declaration(data->input))
		add_var_declaration(data);
	else if (ft_lexer(data))
		make_tree(data);
	if (data->mem_error == 1 || data->redir_err == 2)
	{
		if (data->mem_error == 1)
			error(&data->status, "malloc", MALLOC, 1);
		return (1);
	}
	else if (data->syntax_err)
	{
		if (data->syntax_err == 1)
			ft_putendl_fd("minishell$: syntax : close the quotes!", 2);
		else if (data->syntax_err == 2)
			ft_putendl_fd("minishell$: syntax : '||' not allowed", 2);
		return (1);
	}
	return (1);
}

/**
 * @brief Wrapper for input handling
 * @param data The main data stucture for the program
 * @retval 0 (fail)
 * @retval 1 (success)
 */
static int	process_handler(t_data *data)
{
	if (process_input(data) == 0)
	{
		clear_history();
		builtin_exit(data->root, &data->status);
		return (0);
	}
	if (ft_strncmp(data->input, "exit", 4) == 0)
	{
		if (data->input[4] && !ft_isspace(data->input[4]))
			return (1);
		builtin_exit(data->root, &data->status);
		return (0);
	}
	return (1);
}

/**
 * @brief Main loop for the program giving the prompt for input
 * @param argc Count of the arguments in the program launch
 * @param argv Arguments in the program launch
 * @param envp Environment variables
 * @retval 1 (Failure)
 * @retval data.status.exit_code (Last status code of the program)
 */
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	splash_screen();
	if (!init_base(&data, argc, argv, envp))
	{
		error(&data.status, "malloc", MALLOC, 1);
		destroy_memory(&data);
		return (1);
	}
	while (1)
	{
		init_data(&data);
		rl_event_hook = reset_readline;
		setup_shell_signals();
		if (!process_handler(&data))
			break ;
		if (data.root)
			execute_command(data.root, &data);
		if (data.status.exit_code > 128)
			handle_signal_error(&data.status);
	}
	arena_free(data.env_arena);
	destroy_memory(&data);
	return (data.status.exit_code);
}
