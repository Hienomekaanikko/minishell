/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:51:48 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/14 15:51:53 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo()
{
	printf("PLACEHOLDER: echo\n");
	/*
	Skip the first argument (which is "echo" itself)
	no args "\n"
	Check if args[1] == "-n" -> (no /n at the end)
	Print the remaining arguments with spaces between them
	Add a newline at the end (unless -n flag is used)
	*/
}
void	builtin_cd()
{
	printf("PLACEHOLDER: cd\n");
}
void	builtin_pwd()
{
	printf("PLACEHOLDER: pwd\n");
}
void	builtin_export()
{
	printf("PLACEHOLDER: export\n");
}
void	builtin_unset()
{
	printf("PLACEHOLDER: unset\n");
}
void	builtin_env()
{
	printf("PLACEHOLDER: env\n");
}
