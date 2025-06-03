/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 09:13:12 by mbonsdor          #+#    #+#             */
/*   Updated: 2025/06/03 15:22:09 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	splash_screen(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(1, "\033[2J\033[H", 7);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("  __  __ __  __ __  __ _____ _   _ _____  _____ _", 1);
		ft_putstr_fd("     ______ _      _      \n", 1);
		ft_putstr_fd(" |  \\/  |  \\/  |  \\/  |_   _| \\ | |_   _|/ __", 1);
		ft_putstr_fd("__| |  | |  ____| |    | |      \n", 1);
		ft_putstr_fd(" | \\  / | \\  / | \\  / | | | |  \\| | | | | (__", 1);
		ft_putstr_fd("_ | |__| | |__  | |    | |      \n", 1);
		ft_putstr_fd(" | |\\/| | |\\/| | |\\/| | | | | . ` | | |  \\___ ", 1);
		ft_putstr_fd("\\|  __  |  __| | |    | |      \n", 1);
		ft_putstr_fd(" | |  | | |  | | |  | |_| |_| |\\  |_| |_ ____) | ", 1);
		ft_putstr_fd("|  | | |____| |____| |____  \n", 1);
		ft_putstr_fd(" |_|  |_|_|  |_|_|  |_|_____|_| \\_|_____|______|_", 1);
		ft_putstr_fd("|  |_|______|______| _____| \n", 1);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("			Mikko and Markus Minishell			", 1);
		ft_putstr_fd("					\n", 1);
		ft_putstr_fd("\n", 1);
	}
}
