#include "minishell.h"

int	builtin_echo(char **args, t_exec_status *status)
{
	int		newline_flag;
	int		i;

	if (!args || !args[0])
		return (2);
	i = 0;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
		i++;
	newline_flag = i;
	while(args[i])
	{
		if (ft_strncmp(args[i], "$!", 3) == 0)
			printf("%d", status->pid);
		else if (ft_strncmp(args[i], "$?", 3) == 0)
			printf("%d", status->exit_code);
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline_flag == 0)
		printf("\n");
	return (0);
}
int	builtin_cd()
{
	printf("PLACEHOLDER: cd\n");
	return (0);
}
int	builtin_pwd()
{
	printf("PLACEHOLDER: pwd\n");	
	return (0);
}
int	builtin_export()
{
	printf("PLACEHOLDER: export\n");
	return (0);
}
int	builtin_unset()
{
	printf("PLACEHOLDER: unset\n");
	return (0);
}
int	builtin_env()
{
	printf("PLACEHOLDER: env\n");
	return (0);
}