/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:12:24 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/11 10:46:20 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file exec_error.c
 * @brief Error message handler
 */

/**
 * @brief Gets the correct error msg
 * @param err Type of an error
 * @retval NULL (No such type)
 * @retval msg (Type of an error)
 */
char	*get_error(t_error err)
{
	if (err == AMB)
		return (": ambiguous redirect");
	else if (err == NOT_VALID)
		return (": not a valid identifier");
	else if (err == TOO_MANY)
		return (": too many arguments");
	else if (err == NOENV)
		return (": env not set");
	else if (err == ONLYNUM)
		return (": numeric argument required");
	else if (err == STDOUT)
		return (": failed to redirect stdout");
	else if (err == STDIN)
		return (": failed to redirect stdin");
	else if (err == FAIL)
		return (": failed");
	else if (err == NOPERM)
		return (": Permission denied");
	else if (err == NOFILE)
		return (": No such file or directory");
	else if (err == ISDIR)
		return (": Is a directory");
	else if (err == NOCMD)
		return (": command not found");
	return (NULL);
}

/**
 * @brief Rest of the error msgs
 * @param status The status structure of the program
 */
static void	set_other_status(t_exec_status *status)
{
	status->msg = strerror(errno);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
		status->exit_code = 126;
	else
		status->exit_code = 1;
}

/**
 * @brief Writes the wanted error into a buffer to write it as a one chunk
 * @param status The status structure of the program
 * @param cmd The command that is linked to the error that happened
 * @param err The type of error that is being handled (enum)
 * @param exit_code The exit_code that is being proposed
 * @retval exit_code
 */
int	error(t_exec_status *status, char *cmd, t_error err, int exit_code)
{
	char	buf[512];
	size_t	pos;

	ft_memset(buf, 0, sizeof(buf));
	pos = 0;
	status->signal = 0;
	if (err)
	{
		if (err == MALLOC)
			status->msg = ": Cannot allocate memory";
		else if (err == NOTSET)
			status->msg = " not set";
		else
			status->msg = get_error(err);
		status->exit_code = exit_code;
		pos += ft_strlcat(buf + pos, "minishell: ", sizeof(buf) - pos);
		pos += ft_strlcat(buf + pos, cmd, sizeof(buf) - pos);
		pos += ft_strlcat(buf + pos, status->msg, sizeof(buf) - pos);
		pos += ft_strlcat(buf + pos, "\n", sizeof(buf) - pos);
		write(2, buf, pos);
	}
	else
		set_other_status(status);
	return (exit_code);
}

/**
 * @brief Handles signal error messages
 * @param exec_status The status structure of the program
 */
void	handle_signal_error(t_exec_status *exec_status)
{
	int	status;

	status = exec_status->exit_code - 128;
	if (status == SIGSEGV)
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
		ft_putstr_fd("Quit (core dumped)", 2);
	ft_putstr_fd("\n", 2);
}
