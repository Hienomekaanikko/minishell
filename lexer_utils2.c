/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:51:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 18:17:49 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_lexer *curr, t_lexer *prev)
{
	if (curr->value[0] == '|')
		curr->type = PIPE;
	else if (curr->value[0] == '<' && curr->value[1] == '<')
		curr->type = HERE_DOC;
	else if (curr->value[0] == '<')
		curr->type = RE_IN;
	else if (curr->value[0] == '>' && curr->value[1] == '>')
		curr->type = APPEND_OUT;
	else if (curr->value[0] == '>')
		curr->type = RE_OUT;
	else if (prev->type == PIPE)
		curr->type = CMD;
	else if (prev->type == RE_IN || prev->type == RE_OUT
		|| prev->type == HERE_DOC || prev->type == APPEND_OUT)
		curr->type = ARG;
	else if (prev->type == CMD || prev->type == ARG)
		curr->type = ARG;
}
