/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:53:13 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/17 14:33:47 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//removes all quotes and returns a new cleaned up string
char	*remove_quotes(char *value)
{
	int		i;
	int		j;
	int		len;
	char	*cleaned_value;

	i = 0;
	len = 0;
	cleaned_value = NULL;
	while(value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			i++;
		else
		{
			len++;
			i++;
		}
	}
	cleaned_value = malloc(sizeof(char) * (len + 1));
	if (!cleaned_value)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			i++;
		else
		{
			cleaned_value[j] = value[i];
			i++;
			j++;
		}
	}
	if (value)
	{
		free(value);
		value = NULL;
	}
	cleaned_value[j] = '\0';
	return (cleaned_value);
}

//creates a node with places for childs and args etc data
t_ast	*create_node(char *value, t_token type)
{
	t_ast *new_node = (t_ast*)malloc(sizeof(t_ast));
	new_node->cmd = remove_quotes(value);
	new_node->type = type;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->args = NULL;
	new_node->infile = NULL;
	new_node->outfile = NULL;
	return (new_node);
}

//counts the amount of arguments to allocate right amount of memory for the array of argument strings
int	count_size(t_lexer *current)
{
	t_lexer	*temp;
	int		i;

	i = 0;
	temp = current;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}

//adds arguments with quotes removed
void	add_arguments(t_ast *curr_node, t_lexer *current)
{
	t_lexer	*temp;
	char	*joined;
	int		argument_amount;
	int		i;

	temp = current;
	joined = NULL;
	argument_amount = count_size(temp);
	i = 0;
	curr_node->args = malloc((argument_amount + 1) * sizeof(char *));
	temp = current;
	while (temp && temp->type == ARG)
	{
		curr_node->args[i] = remove_quotes(temp->value);
		i++;
		temp = temp->next;
	}
	curr_node->args[i] = NULL;
}
//adds right child
void	add_right_child(t_ast **position, t_lexer *current)
{
	*position = create_node(current->value, current->type);
	if (current->next && current->next->type == ARG)
		add_arguments(*position, current->next);
}
//adds left child
void	add_left_child(t_ast **position, t_lexer *prev_cmd)
{
	*position = create_node(prev_cmd->value, prev_cmd->type);
	if (*position && prev_cmd->next->type == ARG)
		add_arguments(*position, prev_cmd->next);
}

//makes a tree with pipes or redirections or both
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
				add_left_child(&data->root->left, prev_cmd);
				prev_cmd = NULL;
			}
			if (current->type == CMD)
				add_right_child(&data->root->right, current);
		}
		else if (current->type == PIPE && data->root != NULL)
		{
			new_node = create_node(current->value, current->type);
			new_node->left = data->root;
			current = current->next;
			if (current->type == CMD)
				add_right_child(&new_node->right, current);
			data->root = new_node;
		}
		else if ((current->type == RE_OUT || current->type == RE_IN || current->type == APPEND_OUT || current->type == HERE_DOC) && data->root == NULL)
		{
			data->root = create_node(current->value, current->type);
			current = current->next;
			if (prev_cmd != NULL)
			{
				add_left_child(&data->root->left, prev_cmd);
				prev_cmd = NULL;
			}
			if (current->type == ARG)
				add_right_child(&data->root->right, current);
		}
		else if ((current->type == RE_OUT || current->type == RE_IN || current->type == APPEND_OUT || current->type == HERE_DOC) && data->root != NULL)
		{
			new_node = create_node(current->value, current->type);
			new_node->left = data->root;
			current = current->next;
			if (current->type == ARG)
				add_right_child(&new_node->right, current);
			data->root = new_node;
		}
		if (current)
			current = current->next;
	}
}

//makes a tree without pipes or redirections
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
//determines what kind of tree is needed
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

//launches the tree creation process
void	make_tree(t_data *data)
{
	data->root = NULL;
	if (tree_type(data) == 1)
		set_basic_tree(data);
	else if (tree_type(data) == 2)
		set_complex_tree(data);
	visualize_tree_TEST(data);
}
