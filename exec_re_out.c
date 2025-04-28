#include "minishell.h"

void	exec_re_out(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	int	fd;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		perror("dup");
		return ;
	}
	fd = open(node->right->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if(dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
	execute_command(node->left, env_arena, exec_status, exec_arena);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exec_status->exit_code = 1;
	}
	close(saved_stdout);
}
