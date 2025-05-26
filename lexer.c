/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:10:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 11:06:03 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lexer(t_data *data) //env-arena added
{
	data->temp_array = parser(data->input, ' ');
	if (!data->temp_array)
	{
		data->mem_error = 1;
		return (0);
	}
	if (!ft_make_list(data))
		return (0);
	if (count_dollars(*data->lexed_list))
		check_for_expansions(data);
	if (data->mem_error)
		return (0);
	return (1);
}

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

static void	check_outfiles(t_lexer *checker, char **msg, t_lexer **prev)
{
	if (checker->type == RE_OUT || checker->type == APPEND_OUT)
	{
		if (!(*msg) && checker->type == APPEND_OUT && !(*prev) && !checker->next)
			*msg = "minishell: syntax error near unexpected token `newline'";
		else if (!(*msg) && checker->type == RE_OUT && !checker->next)
			*msg = "minishell: syntax error near unexpected token `newline'";
		else if (!(*msg) && checker->type == APPEND_OUT && !checker->next)
			*msg = "minishell: syntax error near unexpected token `>>'";
		else if (!(*msg) && checker->type == APPEND_OUT && checker->next->type == RE_OUT)
			*msg = "minishell: syntax error near unexpected token `>'";
	}
}

static void	check_infiles(t_lexer *checker, char **msg)
{
	if (checker->type == RE_IN || checker->type == HERE_DOC)
	{
		if (!(*msg) && checker->next && checker->next->type == PIPE)
			*msg = "minishell: syntax error near unexpected token `|'";
		else if (!(*msg) && !checker->next)
			*msg = "minishell: syntax error near unexpected token `newline'";
		else if ((!*msg) && checker->type == RE_IN && checker->next->type != ARG)
			*msg = "minishell: syntax error near unexpected token `<'";
		else if ((!*msg) && checker->type == HERE_DOC && checker->next->type != ARG)
			*msg = "minishell: syntax error near unexpected token `<<'";
	}
}

static void	check_pipes(t_lexer *checker, char **msg, t_lexer **prev)
{
	if (checker->type == PIPE)
	{
		if (!(*msg) && !(*prev))
			*msg = "minishell: syntax error near unexpected token `|'";
		else if (!(*msg) && !checker->next)
			*msg = "minishell: syntax error near unexpected token `|'";
	}
}

//check if the grammar is right
static void	check_grammar_error(t_lexer *checker, char **msg, t_lexer **prev)
{
	check_pipes(checker, msg, prev);
	check_infiles(checker, msg);
	check_outfiles(checker, msg, prev);
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
		ft_putendl_fd(msg, 2);
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
		{
			data->mem_error = 1;
			return (0);
		}
		add_token_type(linked_list);
		input_list++;
	}
	if (!check_grammar(data))
	{
		free_lexed_list(*data->lexed_list);
		data->status.exit_code = 2;
		return (0);
	}
	return (1);
}
