/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 14:29:00 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lexer(t_data *data)
{
	if (!data->input || ft_strlen(data->input) == 0)
		return (0);
	data->temp_array = parser(data, data->input);
	if (!data->temp_array)
		return (0);
	if (!ft_make_list(data))
		return (0);
	if (count_dollars(*data->lexed_list))
		check_for_expansions(data);
	data->status.exit_code = 0;
	if (data->mem_error)
	{
		free_lexed_list(*data->lexed_list);
		return (0);
	}
	return (1);
}

static void	add_token_type(t_lexer **linked_list)
{
	t_lexer	*curr;
	t_lexer	*prev;

	curr = *linked_list;
	prev = curr;
	prev->type = EMPTY;
	while (curr)
	{
		if (prev->type == EMPTY)
			add_starting_token(curr);
		else
			add_token(curr, prev);
		prev = curr;
		curr = curr->next;
	}
}

static int	check_syntax(t_data *data)
{
	t_lexer	*checker;
	t_lexer	*prev;
	char	*msg;

	checker = *data->lexed_list;
	prev = NULL;
	msg = NULL;
	while (checker)
	{
		check_syntax_error(checker, &msg, &prev);
		checker = checker->next;
	}
	if (msg)
	{
		ft_putendl_fd(msg, 2);
		return (0);
	}
	return (1);
}

int	ft_make_list(t_data *data)
{
	t_lexer	**linked_list;
	char	**input_list;

	linked_list = data->lexed_list;
	input_list = data->temp_array;
	while (*input_list)
	{
		if (!ft_add_node(linked_list, *input_list))
		{
			data->mem_error = 1;
			return (0);
		}
		add_token_type(linked_list);
		input_list++;
	}
	if (!check_syntax(data))
	{
		free_lexed_list(*data->lexed_list);
		data->status.exit_code = 2;
		return (0);
	}
	return (1);
}
