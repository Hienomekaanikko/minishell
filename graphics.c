#include "minishell.h"


void	splash_screen(void)
{
	if (isatty(STDIN_FILENO))
	{
		write(1, "\033[2J\033[H", 7);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(" __  __ __  __ __  __ _____ _   _ _____  _____ _    _ ______ _      _      \n", 1);
		ft_putstr_fd(" |  \\/  |  \\/  |  \\/  |_   _| \\ | |_   _|/ ____| |  | |  ____| |    | |      \n", 1);
		ft_putstr_fd(" | \\  / | \\  / | \\  / | | | |  \\| | | | | (___ | |__| | |__  | |    | |      \n", 1);
		ft_putstr_fd(" | |\\/| | |\\/| | |\\/| | | | | . ` | | |  \\___ \\|  __  |  __| | |    | |      \n", 1);
		ft_putstr_fd(" | |  | | |  | | |  | |_| |_| |\\  |_| |_ ____) | |  | | |____| |____| |____  \n", 1);
		ft_putstr_fd(" |_|  |_|_|  |_|_|  |_|_____|_| \\_|_____|______|_|  |_|______|______| _____| \n", 1);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("			Mikko and Markus Minishell								\n", 1);
		ft_putstr_fd("\n", 1);
	}
}
