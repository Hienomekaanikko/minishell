/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/14 11:43:08 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

unsigned int	hash(const char* str, int table_size)
{
	unsigned int	hash;

	hash = 0;
	while (*str)
	{
		hash = hash * 31 + *str;
		str++;
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

void resize_hashmap(t_hashmap *hashmap)
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

void	handle_expansion(t_data *data, t_hashmap *hashmap) //OR likely better to save the hashmap inside data structure.
{
	//when variables are declared in the program like name="mikko", save the stuff inside the hashmap.
	//then there will be access into that all the way through the program if $name is called.
	//so if '$' inside the input, fetch it via "lookup" when excecuting the program
}

int main()
{
	// Create a new hashmap with an initial size of 4
	t_hashmap *hashmap = create_hashmap(4);

	// Insert some key-value pairs
	insert(hashmap, "name", "Mikko");
	insert(hashmap, "age", "31");
	insert(hashmap, "school", "hive");

	// Lookup some values
	printf("name = %s\n", lookup(hashmap, "name"));  // Should print "tree = three"
	printf("age = %s\n", lookup(hashmap, "age"));  // Should print "apple = fruit"
	printf("school = %s\n", lookup(hashmap, "school"));  // Should print "car = vehicle"

	// Clean up memory
	free_hashmap(hashmap);

	return 0;
}
