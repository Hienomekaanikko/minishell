/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 11:49:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/25 14:25:04 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	make_list(t_data *data)
{
	data->new_str_arr = ft_special_split(data->str, ' ');
	if (!ft_make_list(data->linked_list, data->new_str_arr))
	{
		printf("MALLOC!\n");
		return (0);
	}
	return (1);
}

static void init_data(t_data *data)
{
	data->linked_list = malloc(sizeof(t_list *));
	if (!data->linked_list)
	{
		printf("MALLOC\n");
		return;
	}
	*data->linked_list = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_data(&data);
	(void)argc; //maybe something later
	(void)argv; //maybe something later
	(void)envp; //need this later for finding command paths etc.
	while (1)
	{
		data.str = readline("minishell$ ");
		if (data.str == NULL)
			break;
		if (ft_strncmp(data.str, "exit", 4) == 0)
			break;
		if (!make_list(&data))
		{
			printf("MALLOC!\n");
			return (1);
		}
		// replace this with the lexing, parsing etc.
	}
	return (0);
}
