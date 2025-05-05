#include "minishell.h"

// Get the TMPDIR if set, else default to /tmp
char *get_temp_dir(t_arena *env_arena)
{
	char *tmpdir = arena_getenv(env_arena, "TMPDIR");
	if (!tmpdir)
		return ("/tmp");
	return (tmpdir);
}

// Generate the full path for the heredoc file (just uses fixed name "temp")
char *make_filename(t_arena *env_arena)
{
	char *temp_dir = get_temp_dir(env_arena);
	char *filename = ft_strjoin(temp_dir, "/temp");
	return filename;
}

// Write heredoc input into a temp file
int write_heredoc(t_arena *env_arena, char *delimiter, char **out_path)
{
	char *filename = make_filename(env_arena);
	if (!filename)
		return (-1);

	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("error: cannot create heredoc file\n", 2);
		free(filename);
		return (-1);
	}

	while (1)
	{
		char *line = readline("> ");
		if (!line)
			break;

		// Check for end delimiter
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break;
		}

		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}

	close(fd);
	*out_path = filename;
	printf("%s\n", *out_path);
	return (0);
}
