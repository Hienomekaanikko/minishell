/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:05:43 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/10 14:09:32 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file syntax_err.c
 * @brief After the parser, this looks for the syntax errors inside the lexed linked list, such as wrong grammar
 */

/**
 * @brief Check correct >> > placements
 * @param checker The lexed linked list
 * @param msg Pointer to a variable that holds an error message if such is found
 */

static void	check_outfiles(t_lexer *checker, char **msg)
{
	if (checker->type == RE_OUT || checker->type == APPEND_OUT)
	{
		if (!(*msg) && checker->type == APPEND_OUT && !checker->next)
		{
			*msg = "minishell: syntax error near unexpected token `newline'";
			return ;
		}
		else if (!(*msg) && checker->type == RE_OUT && !checker->next)
		{
			*msg = "minishell: syntax error near unexpected token `newline'";
			return ;
		}
		else if (!(*msg) && checker->type == APPEND_OUT && !checker->next)
		{
			*msg = "minishell: syntax error near unexpected token `>>'";
			return ;
		}
		else if (!(*msg) && checker->type == APPEND_OUT
			&& (checker->next->type != ARG))
			*msg = "minishell: syntax error near unexpected token `>'";
		else if (!(*msg) && checker->type == RE_OUT
			&& (checker->next->type != ARG))
			*msg = "minishell: syntax error near unexpected token `<'";
	}
}

/**
 * @brief Check correct << < placements
 * @param checker The lexed linked list
 * @param msg Pointer to a variable that holds an error message if such is found
 */

static void	check_infiles(t_lexer *checker, char **msg)
{
	if (checker->type == RE_IN || checker->type == HERE_DOC)
	{
		if (!(*msg) && checker->next && checker->next->type == PIPE)
			*msg = "minishell: syntax error near unexpected token `|'";
		else if (!(*msg) && !checker->next)
			*msg = "minishell: syntax error near unexpected token `newline'";
		else if ((!*msg) && checker->type == RE_IN
			&& checker->next->type != ARG)
			*msg = "minishell: syntax error near unexpected token `<'";
		else if ((!*msg) && checker->type == HERE_DOC
			&& checker->next->type != ARG)
			*msg = "minishell: syntax error near unexpected token `<<'";
		else if (!(*msg) && checker->type == RE_IN
			&& checker->next->type == RE_OUT)
			*msg = "minishell: syntax error near unexpected token `>'";
	}
}

/**
 * @brief Check correct | placements
 * @param checker The lexed linked list
 * @param msg Pointer to a variable that holds an error message if such is found
 * @param prev Previous node
 */

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

/**
 * @brief Wrapper to call the syntax error functions from
 * @param checker The lexed linked list
 * @param msg Pointer to a variable that holds an error message if such is found
 * @param prev Previous node
 */

void	check_syntax_error(t_lexer *checker, char **msg, t_lexer **prev)
{
	check_pipes(checker, msg, prev);
	check_infiles(checker, msg);
	check_outfiles(checker, msg);
	*prev = checker;
}
