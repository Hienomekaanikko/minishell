/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 10:27:33 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/25 11:56:35 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*ft_lstnew(void *content)
{
	t_list	*element;

	element = malloc(sizeof(t_list));
	if (!element)
		return (NULL);
	element->content = content;
	element->next = NULL;
	return (element);
}

static int	ft_add_node(t_list **list, char *content)
{
	t_list		*new_node;
	t_list		*temp;
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
//makes a linked list, input must be array of strings
int	ft_make_list(t_list **linked_list, char **content)
{
	//char	*parsed_str; <- maybe this could be the cleaned up *content, to input as a second argument to ft_add_node

	while (*content)
	{
		//maybe parse the content per node here?
		if (!ft_add_node(linked_list, *content))
			return (0);
		//maybe add the type of the token here?
		content++;
	}
	return (1);
}
