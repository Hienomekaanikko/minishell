/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:09:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/02 14:44:06 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arena	*arena_cleanup(char **ptrs, char *memory)
{
	if (ptrs)
		free(ptrs);
	if (memory)
		free(memory);
	return (NULL);
}

void	arena_free(t_arena *arena)
{
	if (arena->memory)
		free(arena->memory);
	if (arena->ptrs)
		free(arena->ptrs);
	if (arena)
		free(arena);
}
