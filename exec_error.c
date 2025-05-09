# include "minishell.h"

int	error_handler(t_exec_status *status, const char *msg, int exit_code)
{
	status->signal = 0;
	if (msg)
	{
		status->error_msg = (char *)msg;
		status->exit_code = exit_code;
	}
	else
	{
		status->error_msg = strerror(errno);
		status->exit_code = exit_code;
	}
	ft_putstr_fd(" ", 2);
	ft_putstr_fd(status->error_msg, 2);
	return (exit_code);
}

void	handle_signal_error(t_exec_status *status, int signal)
{
	status->signal = signal;
	status->exit_code = 128 + signal;
	ft_putstr_fd("minishell: ", 2);
	if (signal == SIGINT)
		ft_putstr_fd("\n", 2);
	else if (signal == SIGSEGV)
		ft_putstr_fd("Segmentation fault", 2);
	else if (signal == SIGBUS)
		ft_putstr_fd("Bus error", 2);
	else if (signal == SIGFPE)
		ft_putstr_fd("Floating point exception", 2);
	else if (signal == SIGILL)
		ft_putstr_fd("Illegal instruction", 2);
	else if (signal == SIGTERM)
		ft_putstr_fd("Terminated", 2);
	else if (signal == SIGKILL)
		ft_putstr_fd("Killed", 2);
	else if (signal == SIGPIPE)
		ft_putstr_fd("Broken pipe", 2);
	else
		ft_putstr_fd("Unknown signal", 2);
	ft_putstr_fd("\n", 2);
}

// void	handle_exit_error(t_exec_status *exec_status, int exit_code)
// {
// 	exec_status->exit_code = exit_code;
// 	if (exit_code == 127)
// 		ft_putstr_fd("Command not found", 2);
// 	else if (exit_code == 126)
// 		ft_putstr_fd("Permission denied", 2);
// 	ft_putstr_fd("\n", 2);
// }

