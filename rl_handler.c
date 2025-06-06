/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:32:36 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/02 16:22:01 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	reset_readline(void)
{
	if (g_interrupted)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		//rl_replace_line("", 0);
		rl_redisplay();
		g_interrupted = 0;
	}
	return (0);
}

int	reset_heredoc_readline(void)
{
	if (g_interrupted)
	{
		rl_redisplay();
		//rl_done = 1;
	}
	return (0);
}
