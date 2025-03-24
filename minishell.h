#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_lexing
{
	int				token;
	int				i_split;
	struct s_lexing	*next;
}	t_lexing;

typedef struct s_data
{
	char	**input;
	char	**new_str;
	int		exit_status;
}	t_data;

#endif

