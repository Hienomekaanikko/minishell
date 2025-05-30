/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:53:13 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/16 15:09:30 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//makes a tree without pipes or redirections
void	set_basic_tree(t_data *data)
{
	t_lexer	*current;

	current = *data->lexed_list;
	if (current)
		add_right_child(&data->root, current, current->type);
}
//determines what kind of tree is needed
int	tree_type(t_data *data)
{
	t_lexer	*temp;

	temp = *data->lexed_list;
	while (temp)
	{
		if (temp->type == PIPE || temp->type == RE_IN || temp->type == RE_OUT || temp->type == HERE_DOC || temp->type == APPEND_OUT)
			return (2);
		temp = temp->next;
	}
	return (1);
}

//launches the tree creation process
void	make_tree(t_data *data, t_arena *env_arena, t_exec_status *status)
{
	data->root = NULL;
	if (tree_type(data) == 1)
		set_basic_tree(data);
	else if (tree_type(data) == 2)
		set_complex_tree(data, env_arena, status);
	free_lexed_list(*data->lexed_list);
	//visualize_tree_TEST(data); //DEBUG
}
