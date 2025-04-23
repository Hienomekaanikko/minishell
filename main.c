/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 14:37:28 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_lexer(t_data *data)
{
	data->temp_array = ft_special_split(data->input, ' ');
	if (!ft_make_list(data))
		return (0);
	if (data->exp->var_list)
		check_for_expansions(data);
	ft_free_split(data->temp_array);
	return (1);
}

static void init_data(t_data *data)
{
	data->root = NULL;
	data->lexed_list = malloc(sizeof(t_lexer *));
	if (!data->lexed_list)
	{
		printf("MALLOC\n");
		return;
	}
	*data->lexed_list = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc; //maybe something later
	(void)argv; //maybe something later
	data.exp = malloc(sizeof(t_exp_data));
	data.exp->var_list = malloc(sizeof(t_var));
	data.exp->var_list = NULL;
	while (1)
	{
		init_data(&data);
		rl_on_new_line();
		data.input = readline("minishell$: ");
		if (data.input == NULL)
			continue;
		else
			add_history(data.input);
		if (ft_strncmp(data.input, "exit", 4) == 0)
			break;
		if (is_var_declaration(data.input))
			add_var_declaration(&data);
		else if (!ft_lexer(&data))
			continue ;
		else
			make_tree(&data);
		if (data.root)
			execute_command(data.root, envp);
	}
	return (0);
}
