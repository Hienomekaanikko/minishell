/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/17 17:36:39 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_dollars(char *value)
{
	int	i;
	int	amount;

	i = 0;
	amount = 0;
	while (value[i])
	{
		if (value[i] == '$')
			amount++;
		i++;
	}
	return (amount);
}

void	extract_keys(t_exp_data *exp, char *value)
{
	int		i;
	int		j;
	int		current;
	int		start;

	i = 0;
	current = 0;
	start = 0;
	exp->dollars = count_dollars(value);
	if (exp->dollars)
	{
		exp->extracted_keys = malloc((exp->dollars + 1) * sizeof(char *));
		if (!exp->extracted_keys)
			return ;
		exp->values = malloc((exp->dollars + 1) * sizeof(char *));
		if (!exp->values)
			return ;
	}
	else
		return ;
	while (value[i])
	{
		if (value[i] == '$')
		{
			j = 0;
			i++;
			start = i;
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
				exp->extracted_keys[current] = malloc((j - i + 1) * sizeof(char));
				ft_strlcpy(exp->extracted_keys[current], value + i, j - i + 1);
			}
			else
			{
				while(value[i] && value[i] != 32 && value[i] != '$' && value[i] != '"')
				{
					i++;
					j++;
				}
				i--;
				exp->extracted_keys[current] = ft_substr(value, start, j);
			}
			current++;
		}
		else
			i++;
	}
	exp->extracted_keys[current] = NULL;
}


void	init_exp(t_exp_data *exp)
{
	exp->brace_in = 0;
	exp->brace_out = 0;
	exp->key = NULL;
	exp->values = NULL;
	exp->extracted_keys = NULL;
	exp->dollars = 0;
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

int	new_length(char *value, char **expanded)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	while (expanded[i])
	{
		j = 0;
		while (expanded[i][j])
		{
			len++;
			j++;
		}
		i++;
	}
	i = 0;
	while (value[i])
	{
		if (value[i] == '$')
		{
			while (value[i] && (value[i] != '$' || value[i] != 32))
				i++;
		}
		i++;
		len++;
	}
	return (len);
}

char	*expand(char *value, char **expanded)
{
	char	*new_value;
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	len = (new_length(value, expanded));
	new_value = malloc(sizeof(char) * len);
	while (value[i])
	{
		if (value[i] == '$')
		{
			i++;
			if (*expanded)
			{
				new_value = ft_strjoin(new_value, *expanded);
				j = j + ft_strlen(*expanded);
			}
			expanded++;
			while (value[i] && value[i] != '$' && !ft_isspace(value[i]))
				i++;
			if (value[i] != '$')
				break;
			else
				i--;
		}
		else
			new_value[i] = value[i];
		i++;
	}
	return (new_value);
}

char	*cleanup(char *value)
{
	char	*new_value;
	int		i;

	i = 0;
	while(value[i] && value[i] != '$')
		i++;
	new_value = malloc((i + 1) * sizeof(char));
	i = 0;
	while(value[i] != '$')
	{
		new_value[i] = value[i];
		i++;
	}
	new_value[i] = '\0';
	free(value);
	return (new_value);
}

void	check_for_expansions(t_data *data)
{
	t_exp_data	exp;
	t_lexer		*temp;
	int			i;

	init_exp(&exp);
	temp = *data->lexed_list;
	while(temp)
	{
		extract_keys(&exp, temp->value);
		if (exp.extracted_keys)
		{
			i = 0;
			while (exp.extracted_keys[i])
			{
				exp.values[i] = lookup(data->exp_map, exp.extracted_keys[i]);
				i++;
			}
			i = 0;
			if (exp.values[i])
				temp->value = expand(temp->value, exp.values);
			else
				temp->value = cleanup(temp->value);
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
	j = i;
	i = 0;
	while (data->input[i])
		i++;
	ft_strlcpy(value, data->input + j + 1, i);
	insert(data->exp_map, key, value);
	free(key);
	free(value);
}
