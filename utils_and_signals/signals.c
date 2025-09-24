/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:33 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/11 10:45:18 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file signals.c
 * @brief Signal handlers
 */

volatile sig_atomic_t	g_interrupted = 0;

/**
 * @brief Handles signal
 * @param sig Mandatory even though set to (void) immediately
 */

void	sigint_handler(int sig)
{
	(void)sig;
	g_interrupted = 1;
}

/**
 * @brief Sets up shell signals
 */

void	setup_shell_signals(void)
{
	//rl_event_hook = reset_readline;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	g_interrupted = 0;
}

/**
 * @brief Sets up child signals
 */

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Sets up here_doc signals
 */

void	setup_heredoc_signals(void)
{
	//rl_event_hook = reset_heredoc_readline;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	g_interrupted = 0;
}

/**
 * @brief Ignores signals
 */

void	ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
