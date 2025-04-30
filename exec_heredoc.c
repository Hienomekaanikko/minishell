#include "minishell.h"

//TODO: expand line
//TODO: Signal handling

static char	*create_temp_file(t_arena *exec_arena)
{
	char	*generated_filename;
	int		fd;
	int		i;
	char	*base_path;

	base_path = arena_add(exec_arena, "/tmp/minishell_heredoc_");
	if (!base_path)
		return (NULL);
	i = 0;
	while (i < 1000)
	{
		generated_filename = arena_add(exec_arena, ft_itoa(i));
		if (!generated_filename)
			return (NULL);
		generated_filename = arena_add(exec_arena, ft_strjoin(base_path, generated_filename));
		if (!generated_filename)
			return (NULL);
		fd = open(generated_filename, O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (fd != -1)
		{
			close(fd);
			return (generated_filename);
		}
		i++;
	}
	return (NULL);
}

static int	handle_heredoc_error(int fd, int saved_fd, char *heredoc_file, t_exec_status *status)
{
	if (fd != -1)
		close(fd);
	if (saved_fd != -1)
		close(saved_fd);
	if (heredoc_file)
		unlink(heredoc_file);
	status->exit_code = 1;
	return (1);
}

static int	write_heredoc_to_file(char *heredoc_file, char *delimiter, t_arena *exec_arena)
{
	int		fd;
	char	*line;
	char	*expanded_line;

	fd = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		expanded_line = expand_line(line, exec_arena);
		if (!expanded_line)
		{
			free(line);
			close(fd);
			return (-1);
		}
		write(fd, expanded_line, ft_strlen(expanded_line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (0);
}

int	exec_heredoc(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
	int		fd;
	int		saved_stdin;
	char	*heredoc_file;

	heredoc_file = create_temp_file(exec_arena);
	if (!heredoc_file)
		return (handle_heredoc_error(-1, -1, NULL, status));
	if (write_heredoc_to_file(heredoc_file, node->right->cmd, exec_arena) == -1)
		return (handle_heredoc_error(-1, -1, heredoc_file, status));
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (handle_heredoc_error(-1, -1, heredoc_file, status));
	fd = open(heredoc_file, O_RDONLY);
	if (fd == -1)
		return (handle_heredoc_error(-1, saved_stdin, heredoc_file, status));
	if (dup2(fd, STDIN_FILENO) == -1)
		return (handle_heredoc_error(fd, saved_stdin, heredoc_file, status));
	execute_command(node->left, env_arena, status, exec_arena);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		return (handle_heredoc_error(fd, saved_stdin, heredoc_file, status));
	close(fd);
	close(saved_stdin);
	unlink(heredoc_file);
	return (0);
}
