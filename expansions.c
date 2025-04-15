/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/14 17:10:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"

//work this around

char	*expand(char	*value)
{
	int		i;
	int		j;
	int		y;
	int		brace_in;
	int		brace_out;
	char	*key;

	i = 0;
	brace_in = 0;
	brace_out = 0;
	key = NULL;
	while (value[i])
	{
		if (value[i] == '$')
		{
			i++;
			if (value[i] == '{')
			{
				i++;
				brace_in = 1;
				j = i;
				while (value[j] && value[j] != 32 && value[j] != '$')
				{
					if (brace_in == 1 && value[j] == '}')
					{
						brace_out = 1;
						break;
					}
					j++;
				}
			}
			if (brace_in && brace_out)
			{
				y = j;
				while (value[y] && value[y] != 32 && value[y] != '$')
					y++;
				key = malloc(sizeof(char) * ((j - i) + (y - j)));
				ft_strlcpy(key, value + i, j - i + 1);
				ft_strlcpy(key + j - i, value + j + 1, y - j);

			}
			else
			{
				key = malloc((j - i + 1) * sizeof(char));
				if (!key)
					return (NULL);
				ft_strlcpy(key, value + i, j - i);
			}
		}
		i++;
	}
	return (key);
}

void	check_for_expansions(t_data *data)
{
	t_lexer	*temp;
	char	*key;

	key = NULL;
	temp = *data->lexed_list;
	while(temp)
	{
		key = expand(temp->value);
		if (key)
		{
			free(temp->value);
			temp->value = NULL;
			temp->value = lookup(data->exp_map, key);
			key = NULL;
		}
		temp = temp->next;
	}
}

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

//add variable declaration to hashmap so that its easy to find
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
	printf("key: %s\n", key);
	j = i;
	i = 0;
	while (data->input[i])
		i++;
	ft_strlcpy(value, data->input + j + 1, i);
	insert(data->exp_map, key, value);
	free(key);
	free(value);
}

// int main()
// {
// 	// Create a new hashmap with an initial size of 4
// 	t_hashmap *hashmap = create_hashmap(4);

// 	// Insert some key-value pairs
// 	insert(hashmap, "name", "Mikko");
// 	insert(hashmap, "age", "31");
// 	insert(hashmap, "school", "hive");

// 	// Lookup some values
// 	printf("name = %s\n", lookup(hashmap, "name"));  // Should print "tree = three"
// 	printf("age = %s\n", lookup(hashmap, "age"));  // Should print "apple = fruit"
// 	printf("school = %s\n", lookup(hashmap, "school"));  // Should print "car = vehicle"

// 	// Clean up memory
// 	free_hashmap(hashmap);

// 	return 0;
// }
