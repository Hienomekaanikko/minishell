/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/16 16:23:31 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_key(t_exp_data *exp, char *value)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	printf("checking %s for expansions\n", value);
	while (value[i])
	{
		if (value[i] == '$')
		{
			i++;
			if (value[i] == '{')
			{
				i++;
				exp->brace_in = 1;
				j = i;
				while (value[j] && value[j] != 32 && value[j] != '$')
				{
					if (exp->brace_in == 1 && value[j] == '}')
					{
						exp->brace_out = 1;
						break;
					}
					j++;
				}
				exp->key = malloc((j - i + 1) * sizeof(char));
				ft_strlcpy(exp->key, value + i, j - i + 1);
			}
			else
			{
				while(value[i] && value[i] != 32 && value[i] != '$' && value[i] != '"')
				{
					i++;
					j++;
				}
				exp->key = malloc((j + 1) * sizeof(char));
				printf("allocated space: %d\n", j + 1);
				printf("index for key: %d\n", i - j);
				if (!exp->key)
					return ;
				ft_strlcpy(exp->key, value + i - j, j + 1);
			}
		}
		i++;
	}
}

void	init_exp(t_exp_data *exp)
{
	exp->brace_in = 0;
	exp->brace_out = 0;
	exp->key = NULL;
	exp->expanded_value = NULL;
	exp->value = NULL;
}

int	count_expanded_size(char *value)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while(value[i])
	{
		if (value[i] == '$' && value[i + 1] == '{')
		{
			while (value[i] && value[i] != '}')
				i++;
		}
		i++;
		len++;
	}
	return (len);
}

char	*brace_expand(char *value, char *expanded)
{
	char	*new_value;
	size_t	len;
	int		i;
	int		j;
	int		y;

	i = 0;
	j = 0;
	y = 0;
	len = count_expanded_size(value);
	len = len + ft_strlen(expanded);
	new_value = malloc(sizeof(char) * len + 1);
	while (value[i])
	{
		if (value[i] == '$')
			i++;
		if (value[i] == '{')
		{
			i++;
			while (expanded[y])
			{
				new_value[j] = expanded[y];
				j++;
				y++;
			}
				while (value[i] && value[i] != '}')
				i++;
		}
		if (value[i] == '}')
			i++;
		else
		{
			new_value[j] = value[i];
			i++;
			j++;
		}
	}
	return (new_value);
}

char	*typical_expand(char *value, char *expanded)
{
	char	*new_value;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	len = 0;
	while(value[i])
	{
		if (value[i] == '$')
			break;
		i++;
		len++;
	}
	i = 0;
	len = len + ft_strlen(expanded);
	printf("new length: %d\n", len);
	new_value = malloc(sizeof(char) * len + 1);
	while(value[i] && value[i] != '$')
	{
		new_value[i] = value[i];
		i++;
	}
	j = 0;
	while(expanded[j])
	{
		new_value[i] = expanded[j];
		j++;
		i++;
	}
	new_value[i] = '\0';
	printf("expanded value is: %s\n", new_value);
	return (new_value);
}

void	check_for_expansions(t_data *data)
{
	t_exp_data	exp;
	t_lexer		*temp;

	init_exp(&exp);
	temp = *data->lexed_list;
	while(temp)
	{
		extract_key(&exp, temp->value);
		printf("key = %s\n", exp.key);
		if (exp.key)
		{
			exp.value = lookup(data->exp_map, exp.key);
			printf("value = %s\n", exp.value);
			if (exp.value == NULL)
			{
				temp = temp->next;
				continue;
			}
			if (exp.brace_in && exp.brace_out)
				temp->value = brace_expand(temp->value, exp.value);
			else
				temp->value = typical_expand(temp->value, exp.value);
			exp.key = NULL;
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
