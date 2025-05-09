#include "minishell.h"

static int	handle_redirection_error(int fd, int saved_fd, t_exec_status *status)
{
	if (fd != -1)
		close(fd);
	if (saved_fd != -1)
		close(saved_fd);
	if (errno == EACCES)
		return (error_handler(status, NULL, ERR_PERMISSION));
	if (errno == ENOENT)
		return (error_handler(status, NULL, ERR_OPEN));
	return (error_handler(status, NULL, ERR_REDIRECT));
}

static int	get_redirection_params(t_ast *node, int *open_flags, int *file_perms, int *std_fd)
{
	if (node->type == RE_OUT)
	{
		*open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else if (node->type == RE_IN)
	{
		*open_flags = O_RDONLY;
		*file_perms = 0;
		*std_fd = STDIN_FILENO;
	}
	else if (node->type == APPEND_OUT)
	{
		*open_flags = O_WRONLY | O_CREAT | O_APPEND;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else
		return (0);
	return (1);
}

int	exec_redir(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
	int	fd;
	int	saved_fd;
	int	open_flags;
	int	file_perms;
	int	std_fd;

	if (!get_redirection_params(node, &open_flags, &file_perms, &std_fd))
		return (0);
	saved_fd = dup(std_fd);
	if (saved_fd == -1)
		return (handle_redirection_error(-1, -1, status));
	fd = open(node->right->cmd, open_flags, file_perms);
	if (fd == -1)
	{
		if (errno == EACCES)
			return (error_handler(status, NULL, ERR_PERMISSION));
		if (errno == ENOENT)
			return (error_handler(status, NULL, ERR_OPEN));
		return (handle_redirection_error(-1, saved_fd, status));
	}
	if (dup2(fd, std_fd) == -1)
		return (handle_redirection_error(fd, saved_fd, status));
	execute_command(node->left, env_arena, status, exec_arena);
	if (dup2(saved_fd, std_fd) == -1)
		return (handle_redirection_error(fd, saved_fd, status));
	close(fd);
	close(saved_fd);
	return (0);
} 