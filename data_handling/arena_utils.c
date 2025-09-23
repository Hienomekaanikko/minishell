/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:09:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:46:43 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file arena_utils.c
 * @brief Helper functions for memory arena (handles only env related memory)
 */

/**
 * @brief Cleans up the structure of the arena
 * @param ptrs Pointer to the location of the arena structure
 * @param memory Pointer to the location of a local area where a possible failure occured
 * @retval NULL (element of type t_arena being set to NULL)
 */
t_arena	*arena_cleanup(char **ptrs, char *memory)
{
	if (ptrs)
		free(ptrs);
	if (memory)
		free(memory);
	return (NULL);
}

/**
 * @brief Cleans up the contents of memory arena
 * @param arena the entire t_arena structure
 */
void	arena_free(t_arena *arena)
{
	if (arena->memory)
		free(arena->memory);
	if (arena->ptrs)
		free(arena->ptrs);
	if (arena)
		free(arena);
}
