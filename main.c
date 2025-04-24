/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/11 14:03:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_lexer(t_data *data)
{
	data->temp_array = ft_special_split(data->input, ' ');
	if (!ft_make_list(data))
		return (0);
	return (1);
}

static void init_data(t_data *data)
{
	data->lexed_list = malloc(sizeof(t_lexer *));
	if (!data->lexed_list)
	{
		printf("MALLOC\n");
		return;
	}
	*data->lexed_list = NULL;
}
//perus maini toistaseks etta voidaan rakennella
void	init_exec_status(t_exec_status *status)
{
	ft_memset(status, 0, sizeof(t_exec_status));
}

int	main(int argc, char **argv, char **envp)
{
	t_data			data;
	t_exec_status	exec_status;
	t_arena			*env_arena;

	(void)argc; //maybe something later
	(void)argv; //maybe something later
	init_exec_status(&exec_status);
	env_arena = init_env_arena(envp);
	//TODO: !env_arena -> error and exit
	while (1)
	{
		init_data(&data);
		rl_on_new_line();
		data.input = readline("minishell$: ");
		if (data.input == NULL)
			break;
		if (ft_strncmp(data.input, "exit", 4) == 0)
			break;
		if (!ft_lexer(&data))
			continue ;
		else
			make_tree(&data);
		execute_command(data.root, env_arena, &exec_status);
	}
	arena_free(env_arena);
	return (0);
}
