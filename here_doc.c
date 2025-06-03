/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/03 10:41:30 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_filename(t_data *data)
{
	static int	counter = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(counter++);
	if (set_mem_error(data, num))
		return (NULL);
	filename = ft_strdup("./heredoc_");
	if (set_mem_error(data, filename))
		return (NULL);
	filename = ft_strjoin_free(filename, num);
	if (set_mem_error(data, filename))
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
	filename = make_filename(data);
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
			data->redir_err = 2;
			g_interrupted = 0;
			free(line);
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
	if (data->redir_err == 2)
		return (-1);
	return (0);
}
