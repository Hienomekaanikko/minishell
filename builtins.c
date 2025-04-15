#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;

	i = 0;
	while(args[i])
	{	
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
	/*
	Skip the first argument (which is "echo" itself)
	no args "\n"
	Check if args[1] == "-n" -> (no /n at the end)
	Print the remaining arguments with spaces between them
	Add a newline at the end (unless -n flag is used)
	*/
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