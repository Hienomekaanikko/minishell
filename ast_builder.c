/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:53:13 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/09 16:12:00 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(char *value, t_token type)
{
	t_ast *new_node = (t_ast*)malloc(sizeof(t_ast));
	new_node->value = ft_strdup(value);
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

void	set_pipe_tree(t_lexer **current, t_ast **node)
{
	t_ast	*cmd_node;
	t_ast	*curr_node;
	t_ast	*new_node;
	t_lexer	*lexer_temp;

	if (!(*node))
		*node = create_node("|", PIPE);
	lexer_temp = *current;
	curr_node = *node;
	while (lexer_temp && lexer_temp->type != PIPE)
	{
		if (lexer_temp->type == CMD)
		{
			cmd_node = create_node(lexer_temp->value, CMD);
			lexer_temp = lexer_temp->next;
			while (lexer_temp && lexer_temp->type == ARG)
			{
				//maybe add the arguments to the node to be available with the commands?
				lexer_temp = lexer_temp->next;
			}
			if (!curr_node->left)
				curr_node->left = cmd_node;
			else
				curr_node->right = cmd_node;
			continue;
		}
		lexer_temp = lexer_temp->next;
	}
	if (lexer_temp && lexer_temp->type == PIPE)
	{
		if (curr_node->type == PIPE)
		{
			new_node = create_node("|", PIPE);
			new_node->left = curr_node;
			lexer_temp = lexer_temp->next;
			set_pipe_tree(&lexer_temp, &new_node);
			*node = new_node;
		}
		else
		{
			lexer_temp = lexer_temp->next;
			set_pipe_tree(&lexer_temp, node);
		}
	}
}

void print_ast(t_ast *node, int level)
{
	int	i;

	i = 0;
	if (node == NULL)
		return;

	// Print spaces to indicate the depth level
	while (i < level)
	{
		printf("	");
		i++;
	}
	// Print the current node's value
	printf("%s\n", node->value);
	// Recursively print left and right children
	if (node->left)
		print_ast(node->left, level + 1);
	if (node->right)
		print_ast(node->right, level + 1);
}

int	count_pipes(t_data *data)
{
	int		i;
	t_lexer	*temp;

	i = 0;
	temp = *data->lexed_list;
	while (temp)
	{
		if (temp->type == PIPE)
			i++;
		temp = temp->next;
	}
	return (i);
}


void	make_ast(t_data *data)
{
	t_ast	*root;
	t_lexer	*current;
	int		pipe_count;

	root = NULL;
	current = *data->lexed_list;
	pipe_count = count_pipes(data);
	set_pipe_tree(&current, &root);
	print_ast(root, 0);
}
