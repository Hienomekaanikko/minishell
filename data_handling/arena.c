/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:11:41 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/12 12:07:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	arena_realloc(t_data *data, t_arena *arena, size_t more_space)
{
	char	*old_memory;
	char	*new_memory;
	size_t	new_size;

	old_memory = arena->memory;
	new_size = arena->mem_size + more_space;
	new_memory = malloc(new_size);
	if (!new_memory)
	{
		data->mem_error = 1;
		return (0);
	}
	ft_memcpy(new_memory, old_memory, arena->mem_used);
	free(old_memory);
	arena->memory = new_memory;
	arena->mem_size = new_size;
	return (1);
}

static int	arena_ptrs_realloc(t_data *data, t_arena *arena)
{
	char	**old_ptrs;
	char	**new_ptrs;
	size_t	new_capacity;

	old_ptrs = arena->ptrs;
	new_capacity = arena->ptr_capacity * 2;
	new_ptrs = malloc(sizeof(char *) * (new_capacity + 1));
	if (!new_ptrs)
	{
		data->mem_error = 1;
		return (0);
	}
	ft_memcpy(new_ptrs, old_ptrs, sizeof(char *) * arena->ptrs_in_use);
	new_ptrs[arena->ptrs_in_use] = NULL;
	free(old_ptrs);
	arena->ptrs = new_ptrs;
	arena->ptr_capacity = new_capacity;
	return (1);
}

t_arena	*arena_init(size_t arena_size, size_t initial_ptrs)
{
	t_arena	*arena;
	char	*memory;
	char	**ptrs;

	arena = NULL;
	memory = NULL;
	ptrs = NULL;
	if (arena_size < 128 || initial_ptrs < 2)
		return (NULL);
	ptrs = malloc(sizeof(char *) * (initial_ptrs + 1));
	if (!ptrs)
		return (arena_cleanup(ptrs, memory));
	memory = malloc(arena_size);
	if (!memory)
		return (arena_cleanup(ptrs, memory));
	arena = malloc(sizeof(t_arena));
	if (!arena)
		return (arena_cleanup(ptrs, memory));
	arena->memory = memory;
	arena->mem_size = arena_size;
	arena->mem_used = 0;
	arena->ptrs = ptrs;
	arena->ptr_capacity = initial_ptrs;
	arena->ptrs_in_use = 0;
	return (arena);
}

char	*arena_add(t_data *data, char *add)
{
	size_t	add_len;
	char	*ptr;

	add_len = ft_strlen(add) + 1;
	if (data->env_arena->mem_used + add_len > data->env_arena->mem_size)
	{
		if (!arena_realloc(data, data->env_arena, data->env_arena->mem_size))
			return (NULL);
		if (data->env_arena->ptrs_in_use >= data->env_arena->ptr_capacity)
		{
			if (!arena_ptrs_realloc(data, data->env_arena))
				return (NULL);
		}
	}
	ptr = data->env_arena->memory + data->env_arena->mem_used;
	ft_memcpy(ptr, add, add_len);
	data->env_arena->ptrs[data->env_arena->ptrs_in_use] = ptr;
	data->env_arena->ptrs_in_use++;
	data->env_arena->ptrs[data->env_arena->ptrs_in_use] = NULL;
	data->env_arena->mem_used += add_len;
	return (ptr);
}
