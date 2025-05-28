/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:09:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 11:10:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arena	*arena_cleanup(char **ptrs, char *memory)
{
	free(ptrs);
	free(memory);
	return (NULL);
}

void	arena_free(t_arena *arena)
{
	free(arena->memory);
	free(arena->ptrs);
	free(arena);
}
