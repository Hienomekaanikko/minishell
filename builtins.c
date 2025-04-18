#include "minishell.h"

int	builtin_echo(char **args, t_exec_status *status)
{
	int		newline_flag;
	int		i;

	if (!args || !args[0])
		return (2);
	i = 1;
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
int	builtin_cd(char **args)
{
	chdir(args[1]);
	return (0);
}
int	builtin_pwd()
{
	char	*pwd;
	pwd = getcwd(NULL, 0);
	if(pwd)
	{
		ft_putstr_fd(pwd, 2);
		ft_putstr_fd("\n", 2);
		free(pwd);
	}
	//TODO: (!pwd) / else -> error
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
int	builtin_env(char **env)
{
	int	i;

	i = 0;
	while(env[i])
	{
		ft_putstr_fd(env[i], 2);
		ft_putstr_fd("\n", 2);
		i++;
	}
	return (0);
}