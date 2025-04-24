/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:23:23 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 16:13:29 by msuokas          ###   ########.fr       */
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
	char	*cleaned_value;

	i = 0;
	len = count_new_len(value);
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
		cleaned_value[j] = '\0';
	}
	return (cleaned_value);
}

//adds arguments (including cmd) to the args variable as split string
void	add_arguments(t_ast *curr_node, t_lexer *current)
{
	t_lexer	*temp;
	int		argument_amount;
	int		i;

	temp = current;
	argument_amount = count_size(temp);
	i = 0;
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
		temp = temp->next;
	}
	curr_node->args[i] = NULL;
}
//adds right child
void	add_right_child(t_ast **position, t_lexer *current)
{
	*position = create_node(current->value, current->type);
	if (*position)
		add_arguments(*position, current);
}
//adds left child
void	add_left_child(t_ast **position, t_lexer *prev_cmd)
{
	*position = create_node(prev_cmd->value, prev_cmd->type);
	if (*position)
		add_arguments(*position, prev_cmd);
}
