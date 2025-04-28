#include "minishell.h"

void	exec_pipe(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return;
	}
	pidL = fork();
	if (pidL == -1)
	{
		perror("fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return;
	}
	if (pidL == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		if (node->left->type == PIPE)
			exec_pipe(node->left, env_arena, exec_status, exec_arena);
		else
			execute_command(node->left, env_arena, exec_status, exec_arena);
		exit(exec_status->exit_code);
	}
	pidR = fork();
	if (pidR == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		execute_command(node->right, env_arena, exec_status, exec_arena);
		exit(exec_status->exit_code);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pidL, NULL, 0);
	waitpid(pidR, NULL, 0);
}
