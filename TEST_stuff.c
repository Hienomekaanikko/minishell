/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TEST_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 12:15:05 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 14:39:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_args(t_ast *node)
{
	int		i;
	char	**list;

	if (!node->args)
		return ;
	list = node->args;
	i = 0;
	while (list[i])
	{
		printf("%s\n", list[i]);
		i++;
	}
}

void	show_arguments_per_cmd(t_data *data)
{
	t_ast	*curr;

	curr = data->root;
	printf("\n");
	while (curr)
	{
		if (curr->args)
		{
			printf("Args for cmd '%s':\n", curr->cmd);
			print_args(curr);
		}
		if (curr->right)
		{
			printf("Args for cmd '%s':\n", curr->right->cmd);
			print_args(curr->right);
		}
		curr = curr->left;
	}
}

void	visualize_tree_TEST(t_data *data)
{
	t_ast	*curr;
	size_t	level;
	size_t	i;

	curr = data->root;
	level = 0;
	i = 0;
	while (curr)
	{
		while (i < level)
		{
			printf("    ");
			i++;
		}
		i = 0;
		printf("%s", curr->cmd);
		printf("   ");
		if (curr->right)
			printf("%s\n", curr->right->cmd);
		curr = curr->left;
		level++;
	}
	printf("\n");
	show_arguments_per_cmd(data);
}
