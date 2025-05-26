/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 13:07:24 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void init_data(t_data *data)
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
	if (!data->tools)
	{
		data->tools = malloc(sizeof(t_exp_tools));
		ft_memset(data->tools, 0, sizeof(t_exp_tools));
	}
	*data->lexed_list = NULL;
	data->syntax_err = 0;
	data->mem_error = 0;
	data->temp_array = NULL;
	data->status.redir_fail = 0;
}

int	process_input(t_data *data)
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
	if (data->mem_error)
	{
		free_lexed_list(*data->lexed_list);
		error_handler(&data->status, "malloc", "Cannot allocate memory", 1);
		return (0);
	}
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
	data->mem_error = 0;
	data->tools = NULL;
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

void	init_exec_status(t_data *data)
{
	ft_memset(&data->status, 0, sizeof(t_exec_status));
	data->status.infile = -1;
	data->status.outfile = -1;
	data->status.temp_fd = -1;
	data->status.saved_stdout = -1;
}
int	main(int argc, char **argv, char **envp)
{
	t_data			data;

	splash_screen();
	init_base(&data, argc, argv);
	init_exec_status(&data);
	init_env_arena(envp, &data); //TODO error
	setup_signals();
	while (1)
	{
		init_data(&data);
		if (process_input(&data) == 0)
		{
			builtin_exit(data.root, &data.status);
			break ;
		}
		if (ft_strncmp(data.input, "exit", 4) == 0)
		{
			builtin_exit(data.root, &data.status);
			break ;
		}
		if (data.root)
			execute_command(data.root, &data);
	}
	arena_free(data.env_arena);
	destroy_memory(&data);
	return (data.status.exit_code);
}
