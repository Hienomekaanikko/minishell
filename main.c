/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/20 15:37:50 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void init_data(t_data *data, t_exec_status *status)
{
	if (data->temp_array)
		ft_free_split(data->temp_array);
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
	status->infile = -1;
	status->outfile = -1;
	status->temp_fd = -1;
	*data->lexed_list = NULL;
	data->syntax_err = 0;
	data->temp_array = NULL;
	status->redir_fail = 0;
}

int	process_input(t_data *data, t_exec_status *exec_status, t_arena *env_arena)
{
	data->input = readline("minishell$: ");
	add_history(data->input);
	if (is_var_declaration(data->input))
		add_var_declaration(data);
	if (ft_lexer(data, exec_status, env_arena))
		make_tree(data, env_arena, exec_status);
	return (1);
}

void	init_base(t_data *data, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	data->exp = malloc(sizeof(t_exp_data));
	data->exp->var_list = NULL;
	data->root = NULL;
	data->input = NULL;
	data->redir_err = 0;
	data->lexed_list = malloc(sizeof(t_lexer *));
	if (!data->lexed_list)
	{
		printf("MALLOC\n");
		return;
	}
	data->temp_array = NULL;
	*data->lexed_list = NULL;
}

void	init_exec_status(t_exec_status *status)
{
	ft_memset(status, 0, sizeof(t_exec_status));
	status->infile = -1;
	status->outfile = -1;
	status->temp_fd = -1;
	status->saved_stdout = -1;
}
int	main(int argc, char **argv, char **envp)
{
	t_data			data;
	t_exec_status	exec_status;
	t_arena			*env_arena;
	t_arena			*exec_arena;

	splash_screen();
	init_base(&data, argc, argv);
	init_exec_status(&exec_status);
	setup_signals();
	env_arena = init_env_arena(envp);		//TODO: !env_arena -> error and exit
	exec_arena = arena_init(1024, 1024);	//TODO: Not in use atm.
	while (1)
	{
		init_data(&data, &exec_status);
		if (!process_input(&data, &exec_status, env_arena))
			continue ;
		else if (ft_strncmp(data.input, "exit", 4) == 0)
		{
			if (builtin_exit(data.root, &exec_status))
			{
				printf("exit\n");
				break ;
			}
		}
		if (data.root)
			execute_command(data.root, env_arena, &exec_status, exec_arena);
	}
	arena_free(env_arena);
	arena_free(exec_arena);
	destroy_memory(&data);
	return (exec_status.exit_code);
}
