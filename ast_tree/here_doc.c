/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:18:54 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:31:22 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file here_doc.c
 * @brief here_doc handler functions
 */

/**
 * @brief Creates a filename for the here_doc temp file
 * @param data The main data structure of the program
 * @return temp_file_name
 */
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
		return (NULL);
	free(num);
	return (temp_file_name);
}

/**
 * @brief Signal handler for the EOF (ctrl+d)
 * @param data The main data structure of the program
 * @param line Current line to create the error message correctly
 * @param delimiter Delimiter to create the error message correctly
 * @retval 1 (signal handler used)
 * @retval 0 (signal handler not used)
 */
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

/**
 * @brief Handles the expansions in here_doc ($USER or whatever exists in the env)
 * @param data The main data structure of the program
 * @param line Pointer to the current input line
 * @param is_delim_quote Flag to tell if the delimiter has quotes or not. If true, then expansions will not be expanded.
 * @return Pointer to the possibly expanded line (original if not expanded and NULL if error in any point of the program)
 */
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

/**
 * @brief Creates and writes the here_doc and handles everything in regarding to it
 * @param data The main data structure of the program
 * @param delimiter Delimiter to exit here_doc if input line gets matched with it
 * @param out_path Path to the temp file
 * @retval 1 (if success)
 * @retval < 1 (if any errors during the process)
 */
int	write_heredoc(t_data *data, char **delimiter, char **out_path)
{
	char	*line;
	int		is_delim_quote;
	int		fd;

	data->hd_linecount = 0;
	is_delim_quote = handle_delim_quote(data, delimiter);
	setup_heredoc_signals();
	fd = hd_file_setup(data, out_path);
	if (fd < 0)
		return (-1);
	while (1 && is_delim_quote != -1)
	{
		line = readline("> ");
		if (hd_interrupt_eof(data, line, *delimiter))
			break ;
		data->hd_linecount++;
		line = hd_expand_line(data, &line, is_delim_quote);
		if (hd_handle_delimiter(line, *delimiter))
			break ;
		hd_write_line(fd, line);
	}
	close(fd);
	data->rl_linecount = data->rl_linecount + data->hd_linecount;
	setup_shell_signals();
	return (redir_error_check(data));
}
