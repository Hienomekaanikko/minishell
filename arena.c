#include "minishell.h"

void	arena_realloc(t_arena *arena, size_t more_space)
{
	char	*old_memory;
	char	*new_memory;
	size_t	new_size;

	old_memory = arena->memory;
	new_size = arena->size + more_space;
	new_memory = malloc(new_size);
	ft_memcpy(old_memory, new_memory, arena->size);
	free(old_memory);
	arena->memory = new_memory;
	arena->size = new_size;
}

char	*arena_init(t_arena *arena, size_t size, size_t initial_ptrs)
{
	arena->memory = malloc(size);
	arena->size = size;
	arena->used = 0;
}

char	*arena_add(t_arena *arena, char *add)
{
	size_t	size;
	int		i;
	char	*ptr;

	size = ft_strlen(add) + 1;
	if (arena->used + size > arena->size)
		arena_realloc(arena, arena->size);
	ptr = arena->memory + arena->used;
	i = 0;
	while(add[i])
	{
		arena->memory[arena->used] = add[i];
		arena->used++;
		i++;
	}
	arena->memory[arena->used] = '\0';
	return (ptr);
}