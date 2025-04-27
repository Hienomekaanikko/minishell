/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:27:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/24 13:44:51 by msuokas          ###   ########.fr       */
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

int	ft_add_node(t_lexer **list, char *input_list)
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

//add the starting token if there are no previous tokens
void	add_starting_token(t_lexer *curr)
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

//add tokens that are not the first ones
void	add_token(t_lexer *curr, t_lexer *prev)
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
	else if (prev->type == PIPE)
		curr->type = CMD;
	else if (prev->type == RE_IN || prev->type == RE_OUT || prev->type == HERE_DOC || prev->type == APPEND_OUT)
		curr->type = ARG;
	else if (prev->type == CMD || prev->type == ARG)
		curr->type = ARG;
}
