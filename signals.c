/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:33 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/27 20:56:47 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_interrupted = 0;

void	shell_sigint_handler(int signo)
{
	(void)signo;
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("minishell$:", 1);
	// rl_replace_line("", 0);
	// rl_on_new_line();
	// rl_redisplay();
}

void	setup_shell_signals(void)
{
	signal(SIGINT, shell_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	handle_heredoc_sigint(int signo)
{
	(void)signo;
	g_interrupted = 1;
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

