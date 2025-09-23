/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:32:36 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/11 10:45:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file rl_handler.c
 * @brief Handles readline related signals
 */

/**
 * @brief Resets readline signals to base status
 */

int	reset_readline(void)
{
	if (g_interrupted)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_interrupted = 0;
	}
	return (0);
}

/**
 * @brief Resets readline signals in the here_doc to base status
 */


int	reset_heredoc_readline(void)
{
	if (g_interrupted)
	{
		rl_redisplay();
		rl_done = 1;
	}
	return (0);
}
