#include "minishell.h"

void	exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *exec_status)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;

	pipe(pipe_fd);
	pidL = fork();
	if (pidL == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		execute_command(node->left, env_arena, exec_status);
		exit(0);
	}
	pidR = fork();
	if (pidR == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		execute_command(node->right, env_arena, exec_status);
		exit(0);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pidL, NULL, 0);
	waitpid(pidR, NULL, 0);
}
