/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:08:04 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/16 13:32:00 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file here_doc_utils.c
 * @brief here_doc handler helper functions
 */

/**
 * @brief Checks if delimiter has been inputted, to know if the program needs to finish here_doc creation
 * @param line Current input
 * @param delimiter The set delimiter (for example: cat <<EOF ) <- here the EOF is the delimiter that quits here_doc.
 * @retval 1 (delimiter encountered)
 * @retval 0 (no delimiter encountered)
 */
int	hd_handle_delimiter(char *line, char *delimiter)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

/**
 * @brief Creates a temp file for here_doc data
 * @param data Main data structure of the program
 * @param out_path Pointer to a variable that holds the path of the temp file
 * @retval fd (file descriptor to the temp file)
 * @retval -1 (failure during malloc or file descriptor opening)
 */
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
		ft_putstr_fd("open failed at heredoc\n", 2);
		free(temp_file_name);
		return (-1);
	}
	*out_path = temp_file_name;
	return (fd);
}

/**
 * @brief Removes the quotes of the delimiter if existant
 * @param data Main data structure of the program
 * @param delmiter Pointer to a variable that holds delimiter value
 * @retval 1 (successful quote removal)
 * @retval 0 (no quotes present)
 * @retval -1 (malloc failure)
 */
int	handle_delim_quote(t_data *data, char **delimiter)
{
	char	*new_str;

	if (has_quotes(*delimiter))
	{
		new_str = remove_quotes(*delimiter);
		if (set_mem_error(data, new_str))
			return (-1);
		free(*delimiter);
		*delimiter = new_str;
		return (1);
	}
	return (0);
}

/**
 * @brief Writes data inside the here_doc temp file line by line
 * @param fd File descriptor to write into
 * @param line Line to write into the temp file
 */
void	hd_write_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

/**
 * @brief Checks for redirection errors if present
 * @param data The main data structure of the program
 */
int	redir_error_check(t_data *data)
{
	if (data->redir_err == 2)
		return (-1);
	return (0);
}
