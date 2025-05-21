#include "minishell.h"

int	handle_redirection_error(int fd, t_exec_status *status)
{
	if (fd != -1)
		close(fd);
	if (errno == EACCES)
		return (error_handler(status, NULL, "Permission denied", 1));
	return (error_handler(status, NULL, "No such file or directory", 1));
}

static int	get_redirection_params(t_ast *node, int *open_flags, int *file_perms, int *std_fd)
{
	if (node->type == RE_OUT)
	{
		*open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else if (node->type == RE_IN || node->type == HERE_DOC)
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

static void	assign_node_direction(t_ast *node, t_exec_status *status)
{
	if ((node->type == RE_IN || node->type == HERE_DOC) && status->infile == -1)
		status->infile = status->temp_fd;
	if ((node->type == RE_OUT || node->type == APPEND_OUT) && status->outfile == -1 && status->redir_fail == 0)
		status->outfile = status->temp_fd;
}

static void	open_file(t_ast *node, t_exec_status *status, int open_flags, int file_perms)
{
	if (node->type == HERE_DOC)
		status->temp_fd = open(node->file, open_flags, file_perms);
	else
	{
		if (status->redir_fail == 1)
			node->access = 0;
		status->temp_fd = open(node->right->cmd, open_flags, file_perms);
	}
}

int	exec_redir(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
	int	open_flags;
	int	file_perms;
	int	std_fd;

	if (node->access == 0)
	{
		status->redir_fail = 1;
		return (execute_command(node->left, env_arena, status, exec_arena));
	}
	if (!get_redirection_params(node, &open_flags, &file_perms, &std_fd))
		return (0);
	open_file(node, status, open_flags, file_perms);
	if (status->temp_fd == -1)
		return (execute_command(node->left, env_arena, status, exec_arena));
	else
	{
		assign_node_direction(node, status);
		return (execute_command(node->left, env_arena, status, exec_arena));
	}
	return (0);
}
