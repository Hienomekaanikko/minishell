/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:33 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/26 16:23:34 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(int sig)
{
	(void) sig;
	g_interrupted = 0;
}

void	handle_sigint(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("minishell$:", 1);
}

static void init_sigaction(struct sigaction *sa, void (*handler)(int), int sig)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, sig);
	sa->sa_mask = mask;
	sa->sa_handler = handler;
	sa->sa_flags = 0;
}

void	setup_signals(void)
{
	struct	sigaction sa;

	init_sigaction(&sa, SIG_DFL, SIGQUIT);
	sigaction(SIGQUIT, &sa, NULL);
	init_sigaction(&sa, handle_sigint, SIGINT);
	sigaction(SIGINT, &sa, NULL);
}

void	setup_child_signals(void)
{
	struct	sigaction sa;

	init_sigaction(&sa, SIG_DFL, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
}

void	setup_heredoc_signals(void)
{
	struct	sigaction sa;

	init_sigaction(&sa, handle_heredoc, SIGINT);
	sigaction(SIGINT, &sa, NULL);
}
