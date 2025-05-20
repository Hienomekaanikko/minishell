/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/20 09:13:21 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_temp_dir(t_data *data)
{
	char *tmpdir = arena_getenv(data, "TMPDIR");
	if (!tmpdir)
		return ("/tmp");
	return (tmpdir);
}

char	*make_filename(t_data *data)
{
	char *temp_dir = get_temp_dir(data);
	char *filename = ft_strjoin(temp_dir, "/temp");
	return (filename);
}

// Write heredoc input into a temp file
int	write_heredoc(t_data *data, char *delimiter, char **out_path)
{
	char *filename = make_filename(data);
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
	return (0);
}
