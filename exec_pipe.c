#include "minishell.h"

int	cleanup_pipe(int pipe_fd[2], pid_t pidL, pid_t pidR)
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	if (pidR == -1 && pidL > 0)
		kill(pidL, SIGTERM);
	return (0);
}

static void	handle_left_child(int pipe_fd[2], t_ast *node, t_arena *env_arena,
	t_exec_status *exec_status)
{
	setup_child_signals();
	if (exec_status->outfile != -1)
	{
		close(exec_status->outfile);
		exec_status->outfile = -1;
	}
	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		close(pipe_fd[1]);
		exit(error_handler(exec_status, "pipe","failed to redirect stdout", 1));
	}
	close(pipe_fd[1]);
	if (node->left->type == PIPE)
		exec_pipe(node->left, env_arena, exec_status);
	else
		execute_command(node->left, env_arena, exec_status);
	exit(exec_status->exit_code);
}

static void	handle_right_child(int pipe_fd[2], t_ast *node, t_arena *env_arena,
	t_exec_status *exec_status)
{
	setup_child_signals();
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		exit(error_handler(exec_status, "pipe", "failed to redirect stdin", 1));
	}
	close(pipe_fd[0]);
	execute_command(node->right, env_arena, exec_status);
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
		handle_signal_error(exec_status, WTERMSIG(status));
}

static void	wait_right_process(pid_t pidR, t_exec_status *exec_status)
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
		error_handler(exec_status, NULL, NULL, 0);
	}
}

int	exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *exec_status)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;

	pidL = -1;
	pidR = -1;
	if (pipe(pipe_fd) == -1)
		return (error_handler(exec_status, "pipe","failed", 1));
	pidL = fork();
	if (pidL == -1)
		return (cleanup_pipe(pipe_fd, pidL, pidR));
	if (pidL == 0)
		handle_left_child(pipe_fd, node, env_arena, exec_status);
	pidR = fork();
	if (pidR == -1)
		return (cleanup_pipe(pipe_fd, pidL, pidR));
	if (pidR == 0)
		handle_right_child(pipe_fd, node, env_arena, exec_status);
	cleanup_pipe(pipe_fd, pidL, pidR);
	wait_process(pidL, exec_status);
	wait_right_process(pidR, exec_status);
	return (0);
}
