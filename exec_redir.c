#include "minishell.h"

int	handle_redirection_error( t_data *data)
{
	if (data->status->temp_fd != -1)
		close(data->status->temp_fd);

	if (errno == EACCES)
		return (error_handler(data, NULL, "Permission denied", 1));
	return (error_handler(data, NULL, "No such file or directory", 1));
}

static int	get_redirection_params(t_data *data, int *open_flags, int *file_perms, int *std_fd)
{
	if (data->root->type == RE_OUT)
	{
		*open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else if (data->root->type == RE_IN)
	{
		*open_flags = O_RDONLY;
		*file_perms = 0;
		*std_fd = STDIN_FILENO;
	}
	else if (data->root->type == APPEND_OUT)
	{
		*open_flags = O_WRONLY | O_CREAT | O_APPEND;
		*file_perms = 0644;
		*std_fd = STDOUT_FILENO;
	}
	else if (data->root->type == HERE_DOC)
	{
		*open_flags = O_RDONLY;
		*file_perms = 0;
		*std_fd = STDIN_FILENO;
	}
	else
		return (0);
	return (1);
}

static void	assign_node_direction(t_data *data)
{
	if ((data->root->type == RE_IN || data->root->type == HERE_DOC) && data->status->infile == -1)
		data->status->infile = data->status->temp_fd;
	if ((data->root->type == RE_OUT || data->root->type == APPEND_OUT) && data->status->outfile == -1 && data->status->redir_fail == 0)
		data->status->outfile = data->status->temp_fd;
}

static void	open_file(t_data *data, int open_flags, int file_perms)
{
	if (data->root->type == HERE_DOC)
		data->status->temp_fd = open(data->root->file, open_flags, file_perms);
	else
	{
		if (data->status->redir_fail == 1)
			data->root->access = 0;
		data->status->temp_fd = open(data->root->right->cmd, open_flags, file_perms);
	}
}

int	exec_redir(t_data *data)
{
	int	open_flags;
	int	file_perms;
	int	std_fd;

	if (data->root->access == 0)
	{
		data->status->redir_fail = 1;
		return (execute_command(data));
	}
	if (!get_redirection_params(data, &open_flags, &file_perms, &std_fd))
		return (0);
	open_file(data, open_flags, file_perms);
	if (data->status->temp_fd == -1)
		return (execute_command(data));
	else
	{
		assign_node_direction(data);
		return (execute_command(data));
	}
	return (0);
}
