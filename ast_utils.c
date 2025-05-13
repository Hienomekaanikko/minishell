/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:23:23 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/13 16:07:35 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//check if curr->value has any quotes
int	has_quotes(char *value)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '\'' || value[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

//removes all quotes and returns a new cleaned up string
char	*remove_quotes(char *value)
{
	int		i;
	int		j;
	int		len;
	int		quote;
	char	*cleaned_value;

	i = 0;
	len = count_new_len(value);
	quote = 0;
	cleaned_value = malloc(sizeof(char) * (len + 1));
	if (!cleaned_value)
		return (NULL);
	i = 0;
	j = 0;
	while (value[i])
	{
		if (!quote && (value[i] == '\'' || value[i] == '"'))
		{
			quote = value[i];
			i++;
		}
		else if (quote)
		{
			if (quote == value[i])
				i++;
			else
			{
				cleaned_value[j] = value[i];
				i++;
				j++;
			}
		}
		else
		{
			cleaned_value[j] = value[i];
			i++;
			j++;
		}
		cleaned_value[j] = '\0';
	}
	return (cleaned_value);
}

//adds arguments (including cmd) to the args variable as split string
void	add_arguments(t_ast *curr_node, t_lexer *current, t_token type)
{
	t_lexer	*temp;
	//t_lexer	*prev;
	int		argument_amount;
	int		i;

	temp = current;
	argument_amount = count_size(temp);
	i = 0;
	//prev = NULL;
	curr_node->args = malloc((argument_amount + 1) * sizeof(char *));
	if (!curr_node->args)
		return ;
	while (temp && (temp->type == ARG || temp->type == CMD))
	{
		if (has_quotes(temp->value))
			curr_node->args[i] = remove_quotes(temp->value);
		else
			curr_node->args[i] = ft_strdup(temp->value);
		i++;
		//prev = temp;
		temp = temp->next;
		if (type == RE_IN || type == RE_OUT)
		{
			curr_node->args[i] = NULL;
			return ;
		}
	}
	if (temp)
	{
		if (temp->type == RE_IN || temp->type == RE_OUT || temp->type == APPEND_OUT || temp->type == HERE_DOC)
			temp = temp->next;
		temp = temp->next;
		while (temp && temp->type == ARG)
		{
			if (has_quotes(temp->value))
				curr_node->args[i] = remove_quotes(temp->value);
			else
				curr_node->args[i] = ft_strdup(temp->value);
			i++;
			temp = temp->next;
		}
	}
	curr_node->args[i] = NULL;
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
