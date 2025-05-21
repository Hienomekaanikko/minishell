/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:23:23 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/21 16:45:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//removes all quotes and returns a new cleaned up string
char	*remove_quotes(char *value)
{
	t_ast_utils	ast;

	ft_memset(&ast, 0, sizeof(t_ast_utils));
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

void	allocate_arguments(t_ast_utils *ast, t_ast *curr_node, t_lexer **current)
{
	if (has_quotes((*current)->value))
		curr_node->args[ast->i] = remove_quotes((*current)->value);
	else
		curr_node->args[ast->i] = ft_strdup((*current)->value);
	if (!curr_node->args[ast->i])
	{
		ft_free_split(curr_node->args);
		curr_node->args = NULL;
		return ;
	}
	ast->i++;
	(*current) = (*current)->next;
}

//adds arguments (including cmd) to the args variable as split string
void	add_arguments(t_ast *curr_node, t_lexer *current, t_token type)
{
	t_lexer		*temp;
	t_ast_utils	ast;

	ft_memset(&ast, 0, sizeof(t_ast_utils));
	temp = current;
	ast.argument_amount = count_size(temp);
	curr_node->args = malloc((ast.argument_amount + 1) * sizeof(char *));
	if (!curr_node->args)
		return ;
	while (temp && (temp->type == ARG || temp->type == CMD))
		allocate_arguments(&ast, curr_node, &temp);
	if (temp && type != RE_IN && type != RE_OUT)
	{
		if (temp->type == RE_IN || temp->type == RE_OUT
			|| temp->type == APPEND_OUT || temp->type == HERE_DOC)
			temp = temp->next;
		temp = temp->next;
		while (temp && temp->type == ARG)
			allocate_arguments(&ast, curr_node, &temp);
	}
	curr_node->args[ast.i] = NULL;
}
//adds right child
void	add_right_child(t_ast **position, t_lexer *current, t_token type)
{
	*position = create_node(current->value, current->type);
	if (*position)
		add_arguments(*position, current, type);
}
//adds left child
void	add_left_child(t_ast **position, t_lexer *prev_cmd, t_token type)
{
	*position = create_node(prev_cmd->value, prev_cmd->type);
	if (*position)
		add_arguments(*position, prev_cmd, type);
}
