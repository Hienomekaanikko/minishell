/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:23:23 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 13:13:24 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *value)
{
	t_utils	ast;

	ft_memset(&ast, 0, sizeof(t_utils));
	ast.len = count_new_len(value);
	ast.cleaned_value = malloc(sizeof(char) * (ast.len + 1));
	if (!ast.cleaned_value)
		return (NULL);
	while (value[ast.i])
	{
		if (!ast.quote && (value[ast.i] == '\'' || value[ast.i] == '"'))
			ast.quote = value[ast.i++];
		else if (ast.quote)
		{
			if (ast.quote == value[ast.i])
				ast.i++;
			else
				ast.cleaned_value[ast.j++] = value[ast.i++];
		}
		else
			ast.cleaned_value[ast.j++] = value[ast.i++];
		ast.cleaned_value[ast.j] = '\0';
	}
	return (ast.cleaned_value);
}

int	allocate_arguments(t_utils *ast, t_ast *node, t_lexer **current)
{
	if (has_quotes((*current)->value))
		node->args[ast->i] = remove_quotes((*current)->value);
	else
		node->args[ast->i] = ft_strdup((*current)->value);
	if (!node->args[ast->i])
	{
		ft_free_split(node->args);
		node->args = NULL;
		return (0);
	}
	ast->i++;
	(*current) = (*current)->next;
	return (1);
}

void	add_right_child(t_ast **position, t_lexer *current, t_token type)
{
	t_utils	ast;

	ft_memset(&ast, 0, sizeof(t_utils));
	*position = create_node(current->value, current->type);
	if (*position)
		add_args(&ast, *position, current, type);
}

void	add_left_child(t_ast **position, t_lexer *prev_cmd, t_token type)
{
	t_utils	ast;

	ft_memset(&ast, 0, sizeof(t_utils));
	*position = create_node(prev_cmd->value, prev_cmd->type);
	if (*position)
		add_args(&ast, *position, prev_cmd, type);
}
