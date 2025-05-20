#include "minishell.h"

static int	handle_redirection_error(int fd, int saved_fd, t_data *data)
{
	if (fd != -1)
		close(fd);
	if (saved_fd != -1)
		close(saved_fd);
	if (errno == EACCES)
		return (error_handler(data, NULL, "Permission denied", 1));
	return (error_handler(data, NULL, "No such file or directory", 1));
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
	else if (node->type == HERE_DOC)
	{
		*open_flags = O_RDONLY;
		*file_perms = 0;
		*std_fd = STDIN_FILENO;
	}
	else
		return (0);
	return (1);
}

int	exec_redir(t_data *data)
{
	int	saved_fd;
	int	open_flags;
	int	file_perms;
	int	std_fd;
	int	fd;

	if (data->root->access == 0)	
	{
		data->status->redir_fail = 1;
		return (execute_command(data));
	}
	if (!get_redirection_params(data->root, &open_flags, &file_perms, &std_fd))
		return (0);
	saved_fd = dup(std_fd);
	if (saved_fd == -1)
		return (handle_redirection_error(-1, -1, data));
	if (data->root->type == HERE_DOC)
		fd = open(data->root->file, open_flags, file_perms);
	else
	{
		if (data->status->redir_fail == 1)
			data->root->access = 0;
		fd = open(data->root->right->cmd, open_flags, file_perms);
	}
	if (fd == -1)
		return (execute_command(data));
	else
	{
		if ((data->root->type == RE_IN || data->root->type == HERE_DOC) && data->status->infile == -1)
		{
			data->status->infile = fd;
			if (dup2(data->status->infile, std_fd) == -1)
				return (handle_redirection_error(data->status->infile, saved_fd, data));
		}
		if ((data->root->type == RE_OUT || data->root->type == APPEND_OUT) && data->status->outfile == -1 && data->status->redir_fail == 0)
		{
			data->status->outfile = fd;
			if (dup2(data->status->outfile, std_fd) == -1)
				return (handle_redirection_error(data->status->outfile, saved_fd, data));
		}
		execute_command(data);
		close(fd);
	}
	if (dup2(saved_fd, std_fd) == -1)
		return (handle_redirection_error(data->status->infile, saved_fd, data));
	close(saved_fd);
	return (0);
}
