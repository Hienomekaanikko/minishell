/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/14 10:12:52 by msuokas          ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc; //maybe something later
	(void)argv; //maybe something later
	(void)envp; //need this later for finding command paths etc.
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
		// replace this with the lexing, parsing etc.
	}
	return (0);
}
