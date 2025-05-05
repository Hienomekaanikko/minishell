#include "minishell.h"

int	exec_heredoc(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
	int	fd;

	handle_heredoc_signals();
	// Open temp file that was created during parsing
	fd = open(node->file, O_RDONLY);
	if (fd == -1)
		return (error_handler(status, "heredoc: failed to open temp file", 1));

	// Redirect stdin to temp file
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (error_handler(status, "heredoc: failed to redirect stdin", 1));
	}
	close(fd);

	// Execute command
	if (node->left)
		return (execute_command(node->left, env_arena, status, exec_arena));

	// Clean up temp file
	unlink(node->file);
	return (0);
}