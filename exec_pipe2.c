/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:17:26 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/28 15:07:50 by msuokas          ###   ########.fr       */
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

void	wait_right_process(pid_t pidR, t_exec_status *exec_status)
{
	int	status;

	if (pidR <= 0)
		return ;
	waitpid(pidR, &status, 0);
	if (WIFEXITED(status))
	{
		exec_status->exit_code = WEXITSTATUS(status);
		if (exec_status->exit_code == 0)
			exec_status->exit_code = exec_status->final_exit_code;
	}
	else if (WIFSIGNALED(status))
	{
		exec_status->signal = WTERMSIG(status);
		exec_status->exit_code = 128 + exec_status->signal;
	}
}
