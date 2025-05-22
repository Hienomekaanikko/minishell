/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/22 13:55:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_temp_dir(t_arena *env_arena)
{
	char	*tmpdir;

	tmpdir = arena_getenv(env_arena, "TMPDIR");
	if (!tmpdir)
		return ("/tmp");
	return (tmpdir);
}

char	*make_filename(t_arena *env_arena)
{
	char	*temp_dir;
	char	*filename;

	temp_dir = get_temp_dir(env_arena);
	filename = ft_strjoin(temp_dir, "/temp");
	return (filename);
}

// Write heredoc input into a temp file
int	write_heredoc(t_data *data, t_arena *env_arena, char *delimiter, char **out_path, t_exec_status *status)
{
	char	*filename;
	char	*line;
	char	*expanded_line;
	int		fd;

	filename = make_filename(env_arena);
	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("error: cannot create heredoc file\n", 2);
		free(filename);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		expanded_line = expander(data, line, env_arena, status);
		if (expanded_line)
		{
			free(line);
			line = ft_strdup(expanded_line);
			free(expanded_line);
		}
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
