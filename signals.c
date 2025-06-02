/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:33 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/29 13:14:36 by mbonsdor         ###   ########.fr       */
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
	if (isatty(STDIN_FILENO))
		rl_event_hook = reset_readline;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	g_interrupted = 0;
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_heredoc_signals(void)
{
	if (isatty(STDIN_FILENO))
		rl_event_hook = reset_heredoc_readline;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	g_interrupted = 0;
}

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

