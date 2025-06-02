/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:24 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/02 12:50:27 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_error(t_error err)
{
	if (err == AMB)
		return ("ambiguous redirect");
	else if (err == NOT_VALID)
		return ("not a valid identifier");
	else if (err == TOO_MANY)
		return ("too many arguments");
	else if (err == NOENV)
		return ("env not set");
	else if (err == ONLYNUM)
		return ("numeric argument required");
	else if (err == STDOUT)
		return ("failed to redirect stdout");
	else if (err == STDIN)
		return ("failed to redirect stdin");
	else if (err == FAIL)
		return ("failed");
	else if (err == NOPERM)
		return ("Permission denied");
	else if (err == NOFILE)
		return ("No such file or directory");
	else if (err == ISDIR)
		return ("Is a directory");
	else if (err == NOCMD)
		return ("command not found");
	return (NULL);
}

int	error_handler(t_exec_status *status, char *cmd, t_error err, int exit_code)
{
	status->signal = 0;
	if (err)
	{
		if (err == MALLOC)
			status->msg = "Cannot allocate memory";
		else
			status->msg = get_error(err);
		status->exit_code = exit_code;
	}
	else
	{
		status->msg = strerror(errno);
		if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
			status->exit_code = 126;
		else
			status->exit_code = 1;
	}
	ft_putstr_fd("minishell:", 2);
	ft_putstr_fd(" ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(status->msg, 2);
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


