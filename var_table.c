/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:03:22 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/22 17:18:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_var_declaration(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (i > 0 && input[i] == '=' && (ft_isalnum(input[i - 1]) || input[i - 1] == '$'))
			return (1);
		i++;
	}
	return (0);
}

unsigned int	hash(const char *key, int table_size)
{
	unsigned int	hash;

	hash = 0;
	while (*key)
	{
		hash = hash * 31 + *key;
		key++;
	}
	return (hash % table_size);
}
t_hashmap*	create_hashmap(int initial_size)
{
	t_hashmap	*hashmap;
	int			i;

	i = 0;
	hashmap = malloc(sizeof(t_hashmap));
	hashmap->table_size = initial_size;
	hashmap->item_count = 0;
	hashmap->table = malloc(sizeof(t_entry*) * initial_size);
	while (i < initial_size)
	{
		hashmap->table[i] = NULL;
		i++;
	}
	return (hashmap);
}

//add variable declaration to hashmap so that its easy and fast to find
void	add_var_declaration(t_data *data)
{
	int		i;
	int		j;
	char	*key;
	char	*value;

	i = 0;
	j = 0;
	key = ft_strdup("");
	value = ft_strdup("");
	if (!data->exp_map)
		data->exp_map = create_hashmap(5);
	while (data->input[i] && data->input[i] != '=')
		i++;
	ft_strlcpy(key, data->input, i + 1);
	j = i;
	i = 0;
	while (data->input[i])
		i++;
	ft_strlcpy(value, data->input + j + 1, i);
	insert(data->exp_map, key, value);
	free(key);
	free(value);
}
