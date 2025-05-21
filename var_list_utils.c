/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:06:07 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/21 11:51:10 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_declaration(char	*str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && ft_isalnum(str[i]))
		i++;
	while (str[i] && str[i] == '=')
		i++;
	if (str[i] && ft_isalnum(str[i]))
		return (1);
	return (0);
}

//check if key is already declared and replace the value
int	already_declared(t_var *start, char *key, char *value)
{
	t_var	*temp;
	int		key_len;

	temp = start;
	key_len = ft_strlen(key);
	if (key_len == 0)
		return (0);
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, key_len) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}
