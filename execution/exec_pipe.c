/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:42:08 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 10:46:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_left_child(int pipe_fd[2], t_ast *node, t_data *data)
{
	setup_child_signals();
	if (data->status.outfile != -1)
	{
		close(data->status.outfile);
		data->status.outfile = -1;
	}
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close(pipe_fd[1]);
		exit(error(&data->status, "pipe", STDOUT, 1));
	}
	close(pipe_fd[1]);
	if (node->left->type == PIPE)
		exec_pipe(node->left, data);
	else
		execute_command(node->left, data);
	arena_free(data->env_arena);
	destroy_memory(data);
	exit(data->status.exit_code);
}

static void	handle_right_child(int pipe_fd[2], t_ast *node, t_data *data)
{
	setup_child_signals();
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		exit(error(&data->status, "pipe", STDIN, 1));
	}
	close(pipe_fd[0]);
	execute_command(node->right, data);
	arena_free(data->env_arena);
	destroy_memory(data);
	exit(data->status.exit_code);
}

int	exec_pipe(t_ast *node, t_data *data)
{
	int		pipe_fd[2];
	pid_t	pidl;
	pid_t	pidr;

	pidl = -1;
	pidr = -1;
	if (pipe(pipe_fd) == -1)
		return (error(&data->status, "pipe", FAIL, 1));
	pidl = fork();
	if (pidl == -1)
		return (cleanup_pipe(pipe_fd, pidl, pidr));
	if (pidl == 0)
		handle_left_child(pipe_fd, node, data);
	pidr = fork();
	if (pidr == -1)
		return (cleanup_pipe(pipe_fd, pidl, pidr));
	if (pidr == 0)
		handle_right_child(pipe_fd, node, data);
	ignore_signals();
	cleanup_pipe(pipe_fd, pidl, pidr);
	wait_process(pidl, &data->status);
	wait_process(pidr, &data->status);
	return (0);
}
