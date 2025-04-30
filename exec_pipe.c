#include "minishell.h"

void	exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;
	int		status;

	if (pipe(pipe_fd) == -1)
	{
		handle_exec_error(exec_status, "pipe failed", 1);
		return;
	}
	pidL = fork();
	if (pidL == -1)
	{
		handle_exec_error(exec_status, "fork failed", 1);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return;
	}
	if (pidL == 0)
	{
		setup_child_signals();
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			close(pipe_fd[1]);
			exit(1);
		}
		close(pipe_fd[1]);
		if (node->left->type == PIPE)
			exec_pipe(node->left, env_arena, exec_status, exec_arena);
		else
			execute_command(node->left, env_arena, exec_status, exec_arena);
		exit(exec_status->exit_code);
	}
	pidR = fork();
	if (pidR == -1)
	{
		handle_exec_error(exec_status, "fork failed", 1);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		waitpid(pidL, NULL, 0);
		return;
	}
	if (pidR == 0)
	{
		setup_child_signals();
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		{
			close(pipe_fd[0]);
			exit(1);
		}
		close(pipe_fd[0]);
		execute_command(node->right, env_arena, exec_status, exec_arena);
		exit(exec_status->exit_code);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pidL, &status, 0);
	if (WIFEXITED(status))
		exec_status->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_exec_error(exec_status, NULL, 0);
	waitpid(pidR, &status, 0);
	if (WIFEXITED(status))
		exec_status->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		handle_exec_error(exec_status, NULL, 0);
}