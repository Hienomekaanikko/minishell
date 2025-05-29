/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:24 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/05/29 16:33:04 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	error_handler(t_exec_status *status, const char *cmd, const char *msg, int exit_code)
{
	status->signal = 0;
	if (msg)
	{
		status->error_msg = (char *)msg;
		status->exit_code = exit_code;
	}
	else
	{
		status->error_msg = strerror(errno);
		if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
			status->exit_code = 126;
		else if (errno == ENOENT)
			status->exit_code = 127;
		else
			status->exit_code = 1;
	}
	ft_putstr_fd("minishell:", 2);
	ft_putstr_fd(" ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(status->error_msg, 2);
	return (exit_code);
}

void	handle_signal_error(t_exec_status *status, int signal)
{
	status->signal = signal;
	status->exit_code = 128 + signal;
	ft_putstr_fd("minishell: ", 2);
	if (signal == SIGINT)
		ft_putstr_fd("\n", 2);
	else if (signal == SIGSEGV)
		ft_putstr_fd("Segmentation fault", 2);
	else if (signal == SIGBUS)
		ft_putstr_fd("Bus error", 2);
	else if (signal == SIGFPE)
		ft_putstr_fd("Floating point exception", 2);
	else if (signal == SIGILL)
		ft_putstr_fd("Illegal instruction", 2);
	else if (signal == SIGTERM)
		ft_putstr_fd("Terminated", 2);
	else if (signal == SIGKILL)
		ft_putstr_fd("Killed", 2);
	else if (signal == SIGPIPE)
		ft_putstr_fd("Broken pipe", 2);
	else if (signal == SIGQUIT)
	{
		if (WCOREDUMP(status->raw_status))
			printf("Quit (core dumped)\n");
		else
			printf("Quit\n");
	}
	else
		ft_putstr_fd("Unknown signal", 2);
	ft_putstr_fd("\n", 2);
}


