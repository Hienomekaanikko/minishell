#include "minishell.h"

int	cleanup_pipe(int pipe_fd[2], pid_t pidL, pid_t pidR, t_exec_status *status)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	
	if (pidR == -1 && pidL > 0)
		kill(pidL, SIGTERM);
	return (handle_errno_error(status, "fork", errno));
}

static void	handle_left_child(int pipe_fd[2], t_ast *node, t_arena *env_arena, 
	t_exec_status *exec_status, t_arena *exec_arena)
{
	setup_child_signals();
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close(pipe_fd[1]);
		if (errno == EPIPE)
			exit(error_handler(exec_status, NULL, ERR_BROKEN_PIPE));
		exit(handle_errno_error(exec_status, "dup2", errno));
	}
	close(pipe_fd[1]);
	if (node->left->type == PIPE)
		exec_pipe(node->left, env_arena, exec_status, exec_arena);
	else
		execute_command(node->left, env_arena, exec_status, exec_arena);
	exit(exec_status->exit_code);
}

static void	handle_right_child(int pipe_fd[2], t_ast *node, t_arena *env_arena, 
	t_exec_status *exec_status, t_arena *exec_arena)
{
	setup_child_signals();
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		if (errno == EPIPE)
			exit(error_handler(exec_status, NULL, ERR_BROKEN_PIPE));
		exit(handle_errno_error(exec_status, "dup2", errno));
	}
	close(pipe_fd[0]);
	execute_command(node->right, env_arena, exec_status, exec_arena);
	exit(exec_status->exit_code);
}

void	wait_process(pid_t pid, t_exec_status *exec_status)
{
	int	status;

	if (pid <= 0)
		return;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exec_status->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		exec_status->signal = WTERMSIG(status);
		handle_signal_error(exec_status, WTERMSIG(status));
	}
}

static void	wait_right_process(pid_t pidR, t_exec_status *exec_status)
{
	int	status;

	if (pidR <= 0)
		return ;
	waitpid(pidR, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) != 0)
			exec_status->exit_code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		exec_status->signal = WTERMSIG(status);
		handle_signal_error(exec_status, WTERMSIG(status));
	}
}

int	exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;

	pidL = -1;
	pidR = -1;
	if (pipe(pipe_fd) == -1)
		return (handle_errno_error(status, "pipe", errno));
	pidL = fork();
	if (pidL == -1)
		return (cleanup_pipe(pipe_fd, pidL, pidR, status));
	if (pidL == 0)
		handle_left_child(pipe_fd, node, env_arena, status, exec_arena);
	pidR = fork();
	if (pidR == -1)
		return (cleanup_pipe(pipe_fd, pidL, pidR, status));
	if (pidR == 0)
		handle_right_child(pipe_fd, node, env_arena, status, exec_arena);
	cleanup_pipe(pipe_fd, pidL, pidR, status);
	wait_process(pidL, status);
	wait_right_process(pidR, status);
	return (0);
}