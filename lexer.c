/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 14:02:28 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//add the type of token to the node 'type', according to the enum struct.
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

//check if the grammar is right
static void	check_grammar_error(t_lexer *checker, char **msg, t_lexer **prev)
{
	if (checker->type == RE_IN || checker->type == RE_OUT
		|| checker->type == HERE_DOC || checker->type == APPEND_OUT)
	{
		if (!(*msg) && !checker->next)
			*msg = "minishell$: syntax error near unexpected token `newline'";
	}
	if (checker->type == PIPE)
	{
		if (!(*msg) && !prev)
			*msg = "minishell$: syntax error near unexpected token `|'";
		if (checker->next)
		{
			if (!(*msg) && checker->next->type != CMD && checker->next->type != ARG)
				*msg = "minishell$: syntax error near unexpected token `|'";
		}
		else
		{
			if (!(*msg))
				*msg = "minishell$: a prompt should open here for input, not made yet";
		}
	}
	*prev = checker;
}

//basic checks to make sure the order of things is right. whether commands or paths exits will be checked later.
static int	check_grammar(t_data *data)
{
	t_lexer	*checker;
	t_lexer	*prev;
	char	*msg;

	checker = *data->lexed_list;
	prev = NULL;
	msg = NULL;
	while (checker)
	{
		check_grammar_error(checker, &msg, &prev);
		checker = checker->next;
	}
	if (msg)
	{
		printf("%s\n", msg);
		return (0);
	}
	return (1);
}

//makes a linked list, input must be array of strings (currently made with the special split that can handle " and ')
int	ft_make_list(t_data *data)
{
	t_lexer	**linked_list;
	char	**input_list;

	linked_list = data->lexed_list;
	input_list = data->temp_array;
	while (*input_list)
	{
		if (!ft_add_node(linked_list, *input_list))
			return (0);
		add_token_type(linked_list);
		input_list++;
	}
	if (!check_grammar(data))
		return (0);
	// for testing to see what is inside each node:
	// t_lexer	*temp;
	// temp = *linked_list;
	// while (temp)
	// {
	// 	printf("value: %s, type: %d\n", temp->value, temp->type);
	// 	temp = temp->next;
	// }
	return (1);
}
