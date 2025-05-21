#include "minishell.h"

int	exec_heredoc(t_ast *node, t_arena *env_arena, t_exec_status *status)
{
	int	fd;

	handle_heredoc_signals();
	fd = open(node->file, O_RDONLY);
	if (fd == -1)
		return (error_handler(status, "heredoc", "failed to open temp file", 1));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (error_handler(status, "heredoc", "failed to redirect stdin", 1));
	}
	close(fd);
	if (node->left)
		return (execute_command(node->left, env_arena, status));
	unlink(node->file);
	return (0);
}
