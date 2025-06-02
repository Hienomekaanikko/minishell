/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/02 13:00:21 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_temp_dir(t_data *data, t_arena *env_arena)
{
	char	*tmpdir;

	tmpdir = arena_getenv(data, env_arena, "TMPDIR");
	if (data->mem_error)
		return (NULL);
	if (!tmpdir)
		return ("/tmp");
	return (tmpdir);
}

char	*make_filename(t_data *data, t_arena *env_arena)
{
	static int	counter = 0;
	char		*temp_dir;
	char		*num;
	char		*filename;

	temp_dir = get_temp_dir(data, env_arena);
	if (data->mem_error)
		return (NULL);
	num = ft_itoa(counter++);
	if (set_mem_error(data, num))
		return (NULL);
	filename = ft_strjoin(temp_dir, "/heredoc_");
	if (set_mem_error(data, filename))
		return (NULL);
	filename = ft_strjoin_free(filename, num);
	if (set_mem_error(data,filename))
	{
		free(num);
		return (NULL);
	}
	free(num);
	return (filename);
}

int	write_heredoc(t_data *data, char *delimiter, char **out_path)
{
	char	*filename;
	char	*line;
	char	*expanded_line;
	int		delim_quote;
	int		fd;
	int		linecount;

	delim_quote = 0;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		delim_quote = 1;
		delimiter = remove_quotes(delimiter);
	}
	setup_heredoc_signals();
	filename = make_filename(data, data->env_arena);
	if (!filename)
		return (-1);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("error: cannot create heredoc file\n", 2);
		free(filename);
		return (-1);
	}
	linecount = 0;
	while (1)
	{
		line = readline("> ");
		if  (g_interrupted)
		{
			//cleanup here
			g_interrupted = 0;
			break ;
		}
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document at line ", 2);
			ft_putnbr_fd(linecount, 2);
			ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(delimiter, 2);
			ft_putstr_fd("')\n", 2);
			break ;
		}
		linecount++;
		if (ft_strchr(line, '$') && !delim_quote)
		{
			expanded_line = expand(data, data->tools, line);
			if (expanded_line)
			{
				free(line);
				line = ft_strdup(expanded_line);
				if (set_mem_error(data, line))
					break;
			}
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	setup_shell_signals();
	*out_path = filename;
	return (0);
}
