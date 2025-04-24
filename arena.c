#include "minishell.h"

static void	arena_realloc(t_arena *arena, size_t more_space)
{
	char	*old_memory;
	char	*new_memory;
	size_t	new_size;

	old_memory = arena->memory;
	new_size = arena->mem_size + more_space;
	new_memory = malloc(new_size);
	ft_memcpy(new_memory, old_memory, arena->mem_used);
	free(old_memory);
	arena->memory = new_memory;
	arena->mem_size = new_size;
}

static void	arena_ptrs_realloc(t_arena *arena)
{
	char	**old_ptrs;
	char	**new_ptrs;
	size_t	new_capacity;

	old_ptrs = arena->ptrs;
	new_capacity = arena->ptr_capacity * 2;
	new_ptrs = malloc(sizeof(char *) * (new_capacity + 1));
	ft_memcpy(new_ptrs, old_ptrs, sizeof(char *) * arena->ptrs_in_use);
	free(old_ptrs);
	arena->ptrs = new_ptrs;
	arena->ptr_capacity = new_capacity;
}

t_arena	*arena_init(size_t arena_size, size_t initial_ptrs)
{
	t_arena	*arena;

	if (arena_size < 128 || initial_ptrs < 2)
		return (NULL); //TODO error and exit
	arena = malloc(sizeof(t_arena));
	if (!arena)
		return (NULL);
	arena->memory = malloc(arena_size);
	if (!arena->memory)
	{
		free(arena);
		return (NULL);
	}
	arena->ptrs = malloc(sizeof(char *) * (initial_ptrs + 1));
	if (!arena->ptrs)
	{
		free(arena->memory);
		free(arena);
		return (NULL);
	}
	arena->mem_size = arena_size;
	arena->ptr_capacity = initial_ptrs;
	arena->ptrs_in_use = 0;
	arena->mem_used = 0;
	return (arena);
}

char	*arena_add(t_arena *arena, char *add)
{
	size_t	add_len;
	char	*ptr;

	add_len = ft_strlen(add) + 1;
	if (arena->mem_used + add_len > arena->mem_size)
		arena_realloc(arena, arena->mem_size);
	if (arena->ptrs_in_use >= arena->ptr_capacity)
		arena_ptrs_realloc(arena);
	ptr = arena->memory + arena->mem_used;
	ft_memcpy(ptr, add, add_len);
	arena->ptrs[arena->ptrs_in_use] = ptr;
	arena->ptrs_in_use++;
	arena->mem_used += add_len;
	return (ptr);
}

void	arena_clear(t_arena *arena)
{
	arena->mem_used = 0;
	arena->ptrs_in_use = 0;
}

void	arena_free(t_arena *arena)
{
	free(arena->memory);
	free(arena->ptrs);
	free(arena);
}
