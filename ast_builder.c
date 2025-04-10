/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:53:13 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/10 17:40:46 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_node(char *value, t_token type)
{
	t_ast *new_node = (t_ast*)malloc(sizeof(t_ast));
	new_node->cmd = ft_strdup(value);
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->args = NULL;
	new_node->infile = NULL;
	new_node->outfile = NULL;
	return (new_node);
}

void	add_arguments(t_ast *curr_node, t_lexer *current)
{
	char	*temp_str;
	char	*joined;

	temp_str = ft_strdup("");
	joined = NULL;
	while (current && current->type == ARG)
	{
		joined = ft_strjoin(temp_str, current->value);
		free(temp_str);
		temp_str = joined;
		temp_str = ft_strjoin(temp_str, " ");
		current = current->next;
	}
	if (joined == NULL)
		return ;
	curr_node->args = ft_split(temp_str, ' ');
	free(temp_str);
}

//EI OSAA OIKEIN LAITTAA ARGUMENTTEJA VIEL JOS ON MONTA PIPEE

void	set_complex_tree(t_data *data)
{
	t_lexer	*current;
	t_lexer	*prev_cmd;
	t_ast	*new_node;

	current = *data->lexed_list;
	prev_cmd = NULL;
	while (current)
	{
		if (current->type == CMD)
			prev_cmd = current;
		else if (current->type == PIPE && data->root == NULL)
		{
			data->root = create_node(current->value, current->type);
			current = current->next;
			if (prev_cmd != NULL)
			{
				data->root->left = create_node(prev_cmd->value, prev_cmd->type);
				if (data->root->left && prev_cmd->next->type == ARG)
					add_arguments(data->root->left, prev_cmd->next);
				prev_cmd = NULL;
			}
			if (current->type == CMD)
			{
				data->root->right = create_node(current->value, current->type);
				if (current->next && current->next->type == ARG)
					add_arguments(data->root->right, current->next);
			}
		}
		else if (current->type == PIPE &&  data->root != NULL)
		{
			new_node = create_node(current->value, current->type);
			new_node->left = data->root;
			current = current->next;
			if (current->type == CMD)
			{
				new_node->right = create_node(current->value, current->type);
				if (current->next && current->next->type == ARG)
					add_arguments(new_node, current->next);
			}
			data->root = new_node;
		}
		if (current)
			current = current->next;
	}
}

void	set_basic_tree(t_data *data)
{
	t_lexer	*current;

	current = *data->lexed_list;
	while (current)
	{
		if (current->type == CMD)
			data->root = create_node(current->value, CMD);
		else if (current->type == ARG)
		{
			add_arguments(data->root, current);
			return ;
		}
		current = current->next;
	}
}

int	tree_type(t_data *data)
{
	t_lexer	*temp;

	temp = *data->lexed_list;
	while (temp)
	{
		if (temp->type == PIPE || temp->type == RE_IN || temp->type == RE_OUT)
			return (2);
		temp = temp->next;
	}
	return (1);
}

void	make_tree(t_data *data)
{
	printf("Tree structure: \n");
	data->root = NULL;
	if (tree_type(data) == 1)
		set_basic_tree(data);
	else if (tree_type(data) == 2)
		set_complex_tree(data);
	visualize_tree_TEST(data);
}
