#include "minishell.h"

void	exec_pipe(t_ast *node, char **env, t_exec_status *exec_status)
{
	int 	pipe_fd[2];
	pid_t	pidL;
	pid_t	pidR;

	debug_print("Creating pipe");
	pipe(pipe_fd);
	printf("Pipe fds: read=%d, write=%d\n", pipe_fd[0], pipe_fd[1]);
	debug_print("Forking left child");
	pidL = fork();
	if (pidL == 0)
	{
		debug_print("Left child setting up pipe");
		printf("Left child before redirection - stdout=%d\n", STDOUT_FILENO);
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		printf("Left child after redirection - stdout=%d\n", STDOUT_FILENO);
		
		execute_command(node->left, env, exec_status);
		exit(0);
	}

	// Second child (right side)
	debug_print("Forking right child");
	pidR = fork();
	if (pidR == 0)
	{
		debug_print("Right child setting up pipe");
		printf("Right child before redirection - stdin=%d\n", STDIN_FILENO);
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		printf("Right child after redirection - stdin=%d\n", STDIN_FILENO);
		
		execute_command(node->right, env, exec_status);
		exit(0);
	}

	// Parent
	debug_print("Parent closing pipe ends");
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	waitpid(pidL, NULL, 0);
	waitpid(pidR, NULL, 0);
	debug_print("Pipe execution complete");
}
