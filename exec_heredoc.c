#include "minishell.h"

int	exec_heredoc(t_data *data)
{
	int	fd;

	handle_heredoc_signals();
	fd = open(data->root->file, O_RDONLY);
	if (fd == -1)
		return (error_handler(data, "heredoc", "failed to open temp file", 1));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (error_handler(data, "heredoc", "failed to redirect stdin", 1));
	}
	close(fd);
	if (data->root->left)
		return (execute_command(data));
	unlink(data->root->file);
	return (0);
}
