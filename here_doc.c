/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/04 10:20:26 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"

char	*make_temp_file_name(t_data *data)
{
	static int	counter = 0;
	char		*num;
	char		*temp_file_name;

	num = ft_itoa(counter++);
	if (set_mem_error(data, num))
		return (NULL);
	temp_file_name = ft_strdup("./heredoc_");
	if (set_mem_error(data, temp_file_name))
		return (NULL);
	temp_file_name = ft_strjoin_free(temp_file_name, num);
	if (set_mem_error(data, temp_file_name))
	{
		free(num);
		return (NULL);
	}
	free(num);
	return (temp_file_name);
}

int	hd_interrupt_eof(t_data *data, char *line, int linecount, char *delimiter)
{
	if (g_interrupted)
	{
		data->redir_err = 2;
		g_interrupted = 0;
		free(line);
		return (1);
	}
	else if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document at line ", 2);
		ft_putnbr_fd(linecount, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	return (0);
}

char	*hd_expand_line(t_data *data, char **line, int is_delim_quote)
{
	char	*expanded_line;

	if (ft_strchr(*line, '$') && !is_delim_quote)
	{
		expanded_line = expand(data, data->tools, *line);
		if (expanded_line)
		{
			free(*line);
			*line = ft_strdup(expanded_line);
			free(expanded_line);
		}
	}
	return (*line);
}

int	hd_handle_delimiter(char *line, char *delimiter)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

int	hd_file_setup(t_data *data, char **out_path)
{
	int		fd;
	char	*temp_file_name;

	temp_file_name = make_temp_file_name(data);
	if (!temp_file_name)
		return (-1);
	fd = open(temp_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_putstr_fd("error: cannot create heredoc file\n", 2);
		free(temp_file_name);
		return (-1);
	}
	*out_path = temp_file_name;
	return (fd);
}

int	handle_delim_quote(char *delimiter)
{
	if (delimiter[0] == '\'' || delimiter[0] == '"')
	{
		delimiter = remove_quotes(delimiter);
		return (1);
	}
	return (0);
}

void	hd_write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

int	redir_error_check(t_data *data)
{
	if (data->redir_err == 2)
		return (-1);
	return (0);
}

int	write_heredoc(t_data *data, char *delimiter, char **out_path)
{
	char	*line;
	int		is_delim_quote;
	int		fd;
	int		linecount;

	is_delim_quote = handle_delim_quote(delimiter);
	setup_heredoc_signals();
	fd = hd_file_setup(data, out_path);
	if (fd < 0)
		return (-1); //what should i return here?
	linecount = 0;
	while (1)
	{
		line = readline("> ");
		if (hd_interrupt_eof(data, line, linecount, delimiter))
			break ;
		linecount++;
		line = hd_expand_line(data, &line, is_delim_quote);
		if (hd_handle_delimiter(line, delimiter))
			break ;
		hd_write_line(fd, line);
	}
	close(fd);
	setup_shell_signals();
	return (redir_error_check(data));
}
