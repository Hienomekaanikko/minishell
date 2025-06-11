/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:08:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/11 16:00:42 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	handle_delim_quote(char **delimiter)
{
	char	*new_str;

	if ((*delimiter)[0] == '\'' || (*delimiter)[0] == '"')
	{
		new_str = remove_quotes(*delimiter);
		free(*delimiter);
		*delimiter = new_str;
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
