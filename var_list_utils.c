/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_list_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:06:07 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 12:27:21 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	note_quote(char *str, int i, int *has_open_quote)
{
	if (!*has_open_quote)
		*has_open_quote = str[i];
	else if (str[i] == *has_open_quote)
		*has_open_quote = 0;
	else
	{
		ft_putstr_fd("ERROR: Mismatched or unclosed quote\n", 2);
		return (0);
	}
	return (1);
}

//check if var declaration
int	is_var_declaration(char *str)
{
	int		i = 0;
	int		has_open_quote = 0;

	if (!str || ft_strlen(str) == 0)
		return (0);
	while (str[i] && ft_isalnum(str[i]))
		i++;
	if (str[i] != '=')
		return (0);
	i++;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"'))
		{
			if (!note_quote(str, i, &has_open_quote))
				return (0);
		}
		i++;
	}
	if (has_open_quote)
	{
		ft_putstr_fd("ERROR: Unclosed quote\n", 2);
		return (0);
	}
	return (1);
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
