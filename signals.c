/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:33 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/03 15:28:08 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_interrupted = 0;

void	sigint_handler(int sig)
{
	(void)sig;
	g_interrupted = 1;
}

void	setup_shell_signals(void)
{
	//rl_event_hook = reset_readline;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	g_interrupted = 0;
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_heredoc_signals(void)
{
	//rl_event_hook = reset_heredoc_readline;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	g_interrupted = 0;
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
