/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:20:01 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/16 14:40:10 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
#define EXPANSIONS_H

# include "minishell.h"

typedef struct s_exp_data
{
	int		brace_in;
	int		brace_out;
	char	*key;
	char	*value;
	char	*expanded_value;
} t_exp_data;

typedef struct s_entry
{
	char			*key;
	char			*value;
	struct s_entry	*next;
} t_entry;

typedef struct s_hashmap
{
	t_entry	**table;
	int		table_size;
	int		item_count;
} t_hashmap;

int				is_var_declaration(char *input);
void			insert(t_hashmap *hashmap, const char *key, const char *value);
char*			lookup(t_hashmap *hashmap, const char *key);
void			resize_hashmap(t_hashmap *hashmap);
void			free_hashmap(t_hashmap *hashmap);
t_hashmap*		create_hashmap(int initial_size);
unsigned int	hash(const char* str, int table_size);

#endif
