#include "minishell.h"

//how to use heredoc function
// char *heredoc_file;
// if (write_heredoc("EOF", &heredoc_file) == 0) {
//     // `heredoc_file` holds the full path, e.g., "/tmp/temp"
//     printf("Heredoc file: %s\n", heredoc_file);
//     // Now you can open(heredoc_file), dup2 it into stdin, etc.
// }


//maybe find a way to make dynamic filename so no overwriting can happen
char *get_temp_dir(void)
{
	char *tmpdir = arena_getenv(env_arena, "TMPDIR");
	if (!tmpdir)
		return ("/tmp");
	return (tmpdir);
}

// Generate the full path for the temp file (filename = "temp")
char *make_filename(void)
{
	// Get the temporary directory (e.g., "/tmp" or from TMPDIR environment variable)
	char	*temp_dir;
	char	*filename;

	temp_dir = get_temp_dir();
	filename = malloc(ft_strlen(temp_dir) + ft_strlen("/temp") + 1);
	if (!filename)
		return (NULL);
	filename = ft_strdup(temp_dir);
	filename = ft_strjoin(temp_dir, "/temp");
	return (filename);
}

// Reads heredoc input and writes it to a temp file
int write_heredoc(const char *delimiter, char **out_path)
{
	char	*filename;
	char	*line;
	int		fd;
	int		i;

	filename = make_filename();
	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("error: cannot create heredoc file", 2);
		free(filename);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		// Compare with delimiter
		i = 0;
		while (delimiter[i] && line[i] && delimiter[i] == line[i])
			i++;
		if (delimiter[i] == '\0' && line[i] == '\0')
		{
			free(line);
			break;
		}
		// Write line and newline to file
		while (line[i])
		{
			write(fd, line, i);
			write(fd, "\n", 1);
			free(line);
		}
	}
	close(fd);
	*out_path = filename;
	return (0);
}
