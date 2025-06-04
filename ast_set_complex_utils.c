/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:26:47 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 17:45:06 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perms(t_data *data, char *path, int type)
{
	if (path[0] == '$' && type != HERE_DOC)
	{
		(error(&data->status, path, AMB, 1));
		data->mem_error = 2;
		return (-2);
	}
	else if (type == RE_IN)
		return (access(path, R_OK));
	else if (type == RE_OUT || type == APPEND_OUT)
	{
		if (access(path, F_OK) == 0)
			return (access(path, W_OK));
		else
			return (0);
	}
	return (-1);
}

void	add_args(t_utils *ast, t_ast *node, t_lexer *current, t_token type)
{
	t_lexer		*temp;

	temp = current;
	ast->argument_amount = count_size(temp);
	node->args = malloc((ast->argument_amount + 1) * sizeof(char *));
	if (!node->args)
		return ;
	while (temp && (temp->type == ARG || temp->type == CMD))
	{
		if (!allocate_arguments(ast, node, &temp))
			return ;
	}
	if (temp && type != RE_IN && type != RE_OUT)
	{
		if (temp->type != PIPE)
			temp = temp->next;
		temp = temp->next;
		while (temp && temp->type == ARG)
		{
			if (!allocate_arguments(ast, node, &temp))
				return ;
		}
	}
	node->args[ast->i] = NULL;
}
