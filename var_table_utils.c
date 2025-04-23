/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_table_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:06:07 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 10:47:52 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	insert(t_hashmap *hashmap, const char *key, const char *value)
{
	unsigned int	index;
	t_entry			*new_entry;

	index = hash(key, hashmap->table_size);
	new_entry = malloc(sizeof(t_entry));
	new_entry->key = ft_strdup(key);
	new_entry->value = ft_strdup(value);
	new_entry->next = hashmap->table[index];
	hashmap->table[index] = new_entry;
	hashmap->item_count++;
	if (hashmap->item_count > hashmap->table_size * 0.75)
		resize_hashmap(hashmap);
}

void	resize_hashmap(t_hashmap *hashmap)
{
	t_entry			**new_table;
	t_entry 		*entry;
	t_entry			*new_entry;
	unsigned int	new_index;
	int				new_size;
	int				i;

	i = 0;
	new_size = hashmap->table_size * 2;
	new_table = malloc(sizeof(t_entry*) * new_size);
	if (!new_table)
		return ;
	while (i < new_size)
	{
		new_table[i] = NULL;
		i++;
	}
	i = 0;
	while (i < hashmap->table_size)
	{
		entry = hashmap->table[i];
		while (entry)
		{
			new_index = hash(entry->key, new_size);
			new_entry = malloc(sizeof(t_entry));
			new_entry->key = ft_strdup(entry->key);
			new_entry->value = ft_strdup(entry->value);
			new_entry->next = new_table[new_index];
			new_table[new_index] = new_entry;
			entry = entry->next;
		}
		i++;
	}
	free(hashmap->table);
	hashmap->table = new_table;
	hashmap->table_size = new_size;
}

char	*lookup(t_hashmap *hashmap, const char *key)
{
	unsigned int	index;
	t_entry			*entry;

	index = hash(key, hashmap->table_size);
	entry = hashmap->table[index];
	while (entry)
	{
		if (strcmp(entry->key, key) == 0)
			return (entry->value);
		entry = entry->next;
	}
	return (NULL);
}

void	free_hashmap(t_hashmap *hashmap)
{
	t_entry *entry;
	t_entry	*temp;
	int		i;

	i = 0;
	while (i < hashmap->table_size)
	{
		entry = hashmap->table[i];
		while (entry)
		{
			temp = entry;
			entry = entry->next;
			free(temp->key);
			free(temp->value);
			free(temp);
		}
		i++;
	}
	free(hashmap->table);
	free(hashmap);
}
