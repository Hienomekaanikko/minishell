/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 16:33:11 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_input(t_data *data)
{
	data->input = readline("minishell$: ");
	if (data->input == NULL)
		return (0);
	if (ft_strlen(data->input) > 0)
		add_history(data->input);
	if (is_var_declaration(data->input))
		add_var_declaration(data);
	else if (ft_lexer(data))
		make_tree(data);
	if (data->mem_error == 1)
	{
		error_handler(&data->status, "malloc", "Cannot allocate memory", 1);
		return (1);
	}
	return (1);
}

static int	process_handler(t_data *data)
{
	if (process_input(data) == 0)
	{
		builtin_exit(data->root, &data->status);
		return (0);
	}
	if (ft_strncmp(data->input, "exit", 4) == 0)
	{
		builtin_exit(data->root, &data->status);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	splash_screen();
	if (!init_base(&data, argc, argv, envp))
		error_handler(&data.status, "malloc", "Cannot allocate memory", 1);
	while (1)
	{
		init_data(&data);
		rl_event_hook = reset_readline;
		setup_shell_signals();
		if (!process_handler(&data))
			break ;
		if (data.root)
			execute_command(data.root, &data);
	}
	arena_free(data.env_arena);
	destroy_memory(&data);
	return (data.status.exit_code);
}
