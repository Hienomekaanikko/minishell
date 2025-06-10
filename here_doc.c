/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/10 16:40:00 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_temp_file_name(t_data *data)
{
	static int	counter;
	char		*num;
	char		*temp_file_name;

	counter = 0;
	num = ft_itoa(counter++);
	if (set_mem_error(data, num))
		return (NULL);
	temp_file_name = ft_strdup("./heredoc_");
	if (set_mem_error(data, temp_file_name))
	{
		free(num);
		return (NULL);
	}
	temp_file_name = ft_strjoin_free(temp_file_name, num);
	if (set_mem_error(data, temp_file_name))
	{
		free(num);
		return (NULL);
	}
	free(num);
	return (temp_file_name);
}

int	hd_interrupt_eof(t_data *data, char *line, char *delimiter)
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
		ft_putnbr_fd(data->rl_linecount, 2);
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
			if (set_mem_error(data, *line))
				return (NULL);
			free_exp_tools(data);
		}
		if (data->mem_error)
			return (NULL);
	}
	return (*line);
}

int	write_heredoc(t_data *data, char *delimiter, char **out_path)
{
	char	*line;
	int		is_delim_quote;
	int		fd;

	is_delim_quote = handle_delim_quote(delimiter);
	setup_heredoc_signals();
	fd = hd_file_setup(data, out_path);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (hd_interrupt_eof(data, line, delimiter))
			break ;
		data->rl_linecount++;
		line = hd_expand_line(data, &line, is_delim_quote);
		if (hd_handle_delimiter(line, delimiter))
			break ;
		hd_write_line(fd, line);
	}
	close(fd);
	setup_shell_signals();
	return (redir_error_check(data));
}
