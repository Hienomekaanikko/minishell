/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:17:26 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/09 17:12:17 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cleanup_pipe(int pipe_fd[2], pid_t pidL, pid_t pidR)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (pidR == -1 && pidL > 0)
		kill(pidL, SIGTERM);
	return (0);
}

void	wait_process(pid_t pid, t_exec_status *exec_status)
{
	int	status;

	if (pid <= 0)
		return ;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exec_status->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		exec_status->signal = WTERMSIG(status);
		exec_status->exit_code = 128 + exec_status->signal;
	}
}
