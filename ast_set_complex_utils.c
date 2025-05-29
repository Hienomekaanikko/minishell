/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_set_complex_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:26:47 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/29 16:00:43 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	perms(t_data *data, char *path, int type)
{
	if (path[0] == '$' && type != HERE_DOC)
	{
		(error_handler(&data->status, path, AMB, 1));
		data->mem_error = 2;
		return (-2);
	}
	else if (type == RE_IN)
		return (access(path, R_OK));
	else if (type == RE_OUT || type == APPEND_OUT)
	{
		if (access(path, F_OK) == 0)
			return (access(path, W_OK));
		else
			return (0);
	}
	return (-1);
}

void	ast_error_check(t_data *data)
{
	if (data->status.msg && data->status.path)
	{
		if (ft_strncmp(data->status.msg, "No such file or directory", 26) == 0)
			error_handler(&data->status, data->status.path, NOFILE, 1);
		else if (ft_strncmp(data->status.msg, "Permission denied", 18) == 0)
			error_handler(&data->status, data->status.path, NOPERM, 1);
		data->status.path = NULL;
		data->status.msg = NULL;
	}
	if (data->redir_status)
		data->redir_err = 1;
}

void	set_access_err(t_data *data, t_ast *new)
{
	if (new->type != HERE_DOC)
	{
		if (data->redir_err == 0)
		{
			data->status.msg = ft_strdup(strerror(errno));
			data->status.path = ft_strdup(new->right->args[0]);
		}
		new->access = 0;
		data->redir_err = 1;
	}
}
