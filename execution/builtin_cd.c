/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 11:03:54 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/18 11:56:50 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @file builtin_cd.c
 * @brief Command handler for "cd"
 */

/**
 * @brief Prints an error that occured during cd processing
 * @param path Path to the argument of "cd"
 */
static void	cd_error(char *path)
{
	char	*msg;

	if (access(path, F_OK) != 0)
		msg = strerror(errno);
	if (access(path, X_OK) != 0)
		msg = strerror(errno);
	ft_putstr_fd("minishell$: ", 2);
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
}

/**
 * @brief Checks if path exists in the input of "cd"
 * @param data The main data structure of the program
 * @param args The arguments for the cmd "cd"
 * @param path Pointer to the value that holds the path (NULL)
 * @retval 0 (memory error)
 * @retval 1 (success but does not go into if path was found for not)
 */
static int	path_in_input(t_data *data, char **args, char **path)
{
	if (args[1])
	{
		*path = ft_strdup(args[1]);
		if (set_mem_error(data, *path))
			return (0);
	}
	return (1);
}

/**
 * @brief Updates pwd after calling cd
 * @param data The main data structure of the program
 * @retval 0 (success)
 * @retval 127 (no file was found)
 */
static int	update_pwd(t_data *data)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (error(&data->status, "cd", NOFILE, 1));
	arena_set_env(data, "PWD", new_pwd);
	free(new_pwd);
	return (0);
}

/**
 * @brief Count the arguments connected to "cd"
 * @param args Pointer to the array that contains the input "cd"
 * @retval Amount of arguments
 */
static int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

/**
 * @brief Handle the built in cd function
 * @param args Pointer to the array that contains the input "cd"
 * @param data The main data structure of the program
 * @retval 0 (success)
 * @retval < 0 (failure)
 */
int	builtin_cd(char **args, t_data *data)
{
	char	*path;

	path = NULL;
	if (count_args(args) > 2)
		return (error(&data->status, "cd", TOO_MANY, 1));
	if (!path_in_input(data, args, &path))
		return (error(&data->status, "cd: MALLOC", MALLOC, 1));
	if (!path)
	{
		path = arena_getenv(data, data->env_arena, "HOME");
		if (data->mem_error)
			return (0);
		if (!path)
			return (error(&data->status, "cd: HOME", NOTSET, 1));
	}
	if (chdir(path) == -1)
	{
		cd_error(path);
		free(path);
		return (error(&data->status, NULL, NO, 1));
	}
	if (update_pwd(data))
		return (1);
	free(path);
	return (0);
}
