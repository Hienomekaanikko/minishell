/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:24 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/02 11:27:41 by mbonsdor         ###   ########.fr       */
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

void	handle_signal_error(t_exec_status *exec_status)
{
	int	status;

	// if (!WIFSIGNALED(exec_status->raw_status))
	// 	return;
	status = exec_status->exit_code - 128;//WTERMSIG(exec_status->raw_status);
	//exec_status->exit_code = 128 + status;
	//ft_putstr_fd("minishell: ", 2);
	if (status == SIGINT)
		ft_putstr_fd("\n", 2);
	else if (status == SIGSEGV)
		ft_putstr_fd("Segmentation fault", 2);
	else if (status == SIGBUS)
		ft_putstr_fd("Bus error", 2);
	else if (status == SIGFPE)
		ft_putstr_fd("Floating point exception", 2);
	else if (status == SIGILL)
		ft_putstr_fd("Illegal instruction", 2);
	else if (status == SIGTERM)
		ft_putstr_fd("Terminated", 2);
	else if (status == SIGKILL)
		ft_putstr_fd("Killed", 2);
	else if (status == SIGPIPE)
		ft_putstr_fd("Broken pipe", 2);
	else if (status == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	// else
	// 	ft_putstr_fd("Unknown signal", 2);
	//ft_putstr_fd("\n", 2);
}


