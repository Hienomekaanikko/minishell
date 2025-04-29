#include "minishell.h"
#include <errno.h>

void	exec_re_out(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	int	fd;
	int	saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		exec_status->exit_code = 1;
		return;
	}
	fd = open(node->right->cmd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		close(saved_stdout);
		exec_status->exit_code = 1;
		return;
	}
	if(dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		exec_status->exit_code = 1;
		exit(1);
	}
	execute_command(node->left, env_arena, exec_status, exec_arena);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		exec_status->exit_code = 1;
		return;
	}
	close(fd);
	close(saved_stdout);
}
