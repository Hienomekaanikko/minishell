/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 08:49:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/08 08:51:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	char *value;
	struct Node *left;
	struct Node *right;
} Node;

Node	*createNode(char *value)
{
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->value = strdup(value);
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

void printAST(Node *node, int level)
{
	int	i;

	i = 0;
	if (node == NULL) return;
	printAST(node->right, level + 1);
	while (i < level)
	{
		printf("	");
		i++;
	}
	printf("%s\n", node->value);
	printAST(node->left, level + 1);
}

int main()
{
	Node *root = createNode("root");
	root->left = createNode("left");
	root->right = createNode("right");
	root->left->left = createNode("left.left");
	root->left->right = createNode("left.right");
	printf("Abstract Syntax Tree:\n");
	printAST(root, 0);
	return (0);
}
