/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:23:23 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:44:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file ast_utils.c
 * @brief Helper functions for AST building
 */

/**
 * @brief Finds the next command from the lexed linked list
 * @param prev_cmd Pointer to a prev_cmd variable that will hold the previous command if found
 * @param curr The current lexed linked list node being handled
 */
void	find_next_command(t_lexer **prev_cmd, t_lexer *curr)
{
	t_lexer	*temp;

	temp = curr;
	while (temp && temp->type != PIPE)
	{
		if ((temp->type == RE_IN || temp->type == RE_OUT
				|| temp->type == APPEND_OUT || temp->type == HERE_DOC)
			&& temp->next && temp->next->next
			&& temp->next->next->type == ARG)
		{
			*prev_cmd = temp->next->next;
			return ;
		}
		temp = temp->next;
	}
	*prev_cmd = NULL;
}

/**
 * @brief If arguments related to a command are found, this allocates arguments to the AST-node's **args variable
 * @param ast Toolkit that holds iterator tools
 * @param node The current AST-node being handled
 * @param current The current lexed linked list-node being handled
 * @retval 1 (success)
 * @retval 0 (malloc failed)
 */
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

/**
 * @brief Adds right child of a node (usually the command)
 * @param position Is the place of the right child (initialized to NULL)
 * @param current The current lexed linked list-node being handled
 */
void	add_right_child(t_ast **position, t_lexer *current)
{
	t_utils	ast;

	ft_memset(&ast, 0, sizeof(t_utils));
	*position = create_node(current->value, current->type);
	if (*position)
		add_args(&ast, *position, current);
}

/**
 * @brief Adds left child of a node (if prev_cmd exists, then that will be added)
 * @param prev_cmd Previous command that has been encountered in the lexed linked list
 * @param current The current lexed linked list-node being handled
 */
void	add_left_child(t_ast **position, t_lexer *prev_cmd, t_lexer *current)
{
	t_utils	ast;

	if (prev_cmd == NULL)
		find_next_command(&prev_cmd, current);
	if (!prev_cmd)
		return ;
	ft_memset(&ast, 0, sizeof(t_utils));
	*position = create_node(prev_cmd->value, prev_cmd->type);
	if (*position)
		add_args(&ast, *position, prev_cmd);
}
