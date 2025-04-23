/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:27:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 14:36:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lexer	*ft_lstnew(void *input_list)
{
	t_lexer	*element;

	element = malloc(sizeof(t_lexer));
	if (!element)
		return (NULL);
	element->value = input_list;
	element->next = NULL;
	return (element);
}

static int	ft_add_node(t_lexer **list, char *input_list)
{
	t_lexer		*new_node;
	t_lexer		*temp;
	char		*content_copy;
	int			i;

	i = 0;
	content_copy = malloc((ft_strlen(input_list) + 1) * sizeof(char));
	if (!content_copy)
		return (0);
	while(input_list[i])
	{
		content_copy[i] = input_list[i];
		i++;
	}
	content_copy[i] = '\0';
	new_node = ft_lstnew(content_copy);
	if (!new_node)
	{
		free(content_copy);
		return (0);
	}
	if (*list == NULL)
		*list = new_node;
	else
	{
		temp = *list;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
	return (1);
}

//add the type of token to the node 'type', according to the enum struct.
static void	add_token_type(t_lexer **linked_list)
{
	t_lexer	*curr;
	t_lexer	*prev;

	curr = *linked_list;
	prev = curr;
	prev->type = EMPTY;
	while (curr)
	{
		if (prev->type == EMPTY)
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
			else
				curr->type = CMD;
		}
		else if (curr->value[0] == '|')
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
		else if (prev->type == RE_IN || prev->type == RE_OUT || prev->type == HERE_DOC || prev->type == APPEND_OUT)
			curr->type = ARG;
		else if (prev->type == CMD || prev->type == ARG)
			curr->type = ARG;
		prev = curr;
		curr = curr->next;
	}
}

//basic checks to make sure the order of things is right. whether commands or paths exits will be checked later.
int	check_grammar(t_data *data)
{
	t_lexer	*checker;
	t_lexer	*prev;
	char	*msg;

	checker = *data->lexed_list;
	prev = NULL;
	msg = NULL;
	while (checker)
	{
		if (checker->type == RE_IN || checker->type == RE_OUT || checker->type == HERE_DOC || checker->type == APPEND_OUT)
		{
			if (!msg && !checker->next)
				msg = "minishell$: syntax error near unexpected token `newline'";
		}
		if (checker->type == PIPE)
		{
			if (!msg && !prev)
				msg = "minishell$: syntax error near unexpected token `|'";

			if (checker->next)
			{
				if (!msg && checker->next->type != CMD && checker->next->type != ARG)
					msg = "minishell$: syntax error near unexpected token `|'";
			}
			else
			{
				if (!msg)
					msg = "minishell$: a prompt should open here for input, not made yet";
			}
		}
		prev = checker;
		checker = checker->next;
	}
	if (msg)
	{
		printf("%s\n", msg);
		return (0);
	}
	return (1);
}

//makes a linked list, input must be array of strings (currently made with the special split that can handle " and ')
int	ft_make_list(t_data *data)
{
	t_lexer	**linked_list;
	char	**input_list;

	linked_list = data->lexed_list;
	input_list = data->temp_array;
	while (*input_list)
	{
		if (!ft_add_node(linked_list, *input_list))
			return (0);
		add_token_type(linked_list);
		input_list++;
	}
	if (!check_grammar(data))
		return (0);
	// for testing to see what is inside each node:
	t_lexer	*temp;
	temp = *linked_list;
	while (temp)
	{
		printf("value: %s, type: %d\n", temp->value, temp->type);
		temp = temp->next;
	}
	return (1);
}
