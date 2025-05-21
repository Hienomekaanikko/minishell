#include "minishell.h"

int	cleanup_pipe(int pipe_fd[2], pid_t pidL, pid_t pidR)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	if (pidR == -1 && pidL > 0)
		kill(pidL, SIGTERM);
	return (0);
}

static void	handle_left_child(t_data *data, t_ast *node, int pipe_fd[2])
{
	setup_child_signals();
	if (data->status->outfile != -1)
	{
		close(data->status->outfile);
		data->status->outfile = -1;
	}
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close(pipe_fd[1]);
		exit(error_handler(data, "pipe", "failed to redirect stdout", 1));
	}
	close(pipe_fd[1]);
	if (data->root->left->type == PIPE)
		exec_pipe(data, node->left);
	else
		execute_command(data, node->left);
	exit(data->status->exit_code);
}

static void	handle_right_child( t_data *data, t_ast *node, int pipe_fd[2])
{
	setup_child_signals();
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		exit(error_handler(data, "pipe", "failed to redirect stdin", 1));
	}
	close(pipe_fd[0]);
	execute_command(data, node->right);
	exit(data->status->exit_code);
}

void	wait_process(t_data *data, pid_t pid)
{
	int	status;

	if (pid <= 0)
		return;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->status->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_signal_error(data, WTERMSIG(status));
}

static void	wait_right_process( t_data *data, pid_t pidR)
{
	int	status;

	if (pidR <= 0)
		return ;
	waitpid(pidR, &status, 0);
	if (WIFEXITED(status))
	{
		data->status->exit_code = WEXITSTATUS(status);
		if (data->status->exit_code == 0)
			data->status->exit_code = data->status->final_exit_code;
	}
	else if (WIFSIGNALED(status))
	{
		data->status->signal = WTERMSIG(status);
		error_handler(data, NULL, NULL, 0);
	}
}

int	exec_pipe(t_data *data, t_ast *node)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;

	pidL = -1;
	pidR = -1;
	if (pipe(pipe_fd) == -1)
		return (error_handler(data, "pipe", "failed", 1));
	pidL = fork();
	if (pidL == -1)
		return (cleanup_pipe(pipe_fd, pidL, pidR));
	if (pidL == 0)
		handle_left_child(data, node, pipe_fd);
	pidR = fork();
	if (pidR == -1)
		return (cleanup_pipe(pipe_fd, pidL, pidR));
	if (pidR == 0)
		handle_right_child(data, node, pipe_fd);
	cleanup_pipe(pipe_fd, pidL, pidR);
	wait_process(data, pidL);
	wait_right_process(data, pidR);
	return (0);
}
