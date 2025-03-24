#include "libft.h"

int	is_space(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_isspace(cmd[i]))
			i++;
		else
			return (0);
	}
	return (1);
}
