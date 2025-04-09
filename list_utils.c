/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:27:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/09 12:51:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lexer	*ft_lstnew(void *content)
{
	t_lexer	*element;

	element = malloc(sizeof(t_lexer));
	if (!element)
		return (NULL);
	element->value = content;
	element->next = NULL;
	return (element);
}

static int	ft_add_node(t_lexer **list, char *content)
{
	t_lexer		*new_node;
	t_lexer		*temp;
	char		*content_copy;
	int			i;

	i = 0;
	content_copy = malloc((ft_strlen(content) + 1) * sizeof(char));
	if (!content_copy)
		return (0);
	while(content[i])
	{
		content_copy[i] = content[i];
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
			curr->type = CMD;
		else if (curr->value[0] == '|')
			curr->type = PIPE;
		else if (curr->value[0] == '<')
			curr->type = RE_IN;
		else if (curr->value[0] == '>')
			curr->type = RE_OUT;
		else if (prev->type == PIPE)
			curr->type = CMD;
		else if (prev->type == RE_IN || prev->type == RE_OUT)
			curr->type = ARG;
		else if (prev->type == CMD || prev->type == ARG)
			curr->type = ARG;
		prev = curr;
		curr = curr->next;
	}
}

//makes a linked list, input must be array of strings (currently made with the special split that can handle " and ')
int	ft_make_list(t_lexer **linked_list, char **content)
{
	while (*content)
	{
		if (!ft_add_node(linked_list, *content))
			return (0);
		add_token_type(linked_list);
		content++;
	}
	// for testing to see what is inside each node:
	// t_lexer	*temp;
	// temp = *linked_list;
	// while (temp)
	// {
	// 	printf("value: %s, type: %d\n", temp->value, temp->type);
	// 	temp = temp->next;
	// }
	return (1);
}
