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
			ft_putnbr_fd(status->pid, 1);
		else if (ft_strncmp(args[i], "$?", 3) == 0)
			ft_putnbr_fd(status->exit_code, 1);
		else
			ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline_flag == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	builtin_cd(char **args, t_exec_status *status)
{
	if (chdir(args[1]) == -1)
	{
		handle_exec_error(status, 0, "cd: No such file or directory", 1);
		return (1);
	}
	return (0);
}

int	builtin_pwd(t_exec_status *status)
{
	char	*pwd;
	
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		handle_exec_error(status, 0, "pwd: getcwd failed", 1);
		return (1);
	}
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}

int	builtin_export(t_exec_status *status)
{
	(void)status;
	ft_putstr_fd("PLACEHOLDER: export\n", 1);
	return (0);
}

int	builtin_unset(t_exec_status *status)
{
	(void)status;
	ft_putstr_fd("PLACEHOLDER: unset\n", 1);
	return (0);
}

int builtin_env(t_arena *arena, t_exec_status *status)
{
	size_t i;

	if (!arena || !arena->ptrs)
	{
		handle_exec_error(status, 0, "env: environment not set", 1);
		return (1);
	}

	i = 0;
	while (i < arena->ptrs_in_use)
	{
		ft_putstr_fd(arena->ptrs[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}