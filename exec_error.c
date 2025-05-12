# include "minishell.h"

static int is_error_msg(const char *msg, const char *error_msg)
{
	if (!msg || !error_msg)
		return (0);
	return (ft_strncmp((char *)msg, (char *)error_msg, ft_strlen(error_msg)) == 0);
}

int	error_handler(t_exec_status *status, const char *cmd, const char *msg)
{
	status->signal = 0;

	if (is_error_msg(msg, ERR_PERMISSION))
		status->exit_code = 126;
	else if (is_error_msg(msg, ERR_NOT_FOUND))
		status->exit_code = 127;
	else if (is_error_msg(msg, ERR_BROKEN_PIPE))
		status->exit_code = 141;
	else
		status->exit_code = 1;
	
	ft_putstr_fd(ERR_PREFIX, 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (msg)
		ft_putstr_fd(msg, 2);
	else
		ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	return (status->exit_code);
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

int	handle_errno_error(t_exec_status *status, const char *cmd, int errno_val)
{
	if (errno_val == EACCES || errno_val == EPERM || errno_val == EROFS)
		status->exit_code = 126;
	else if (errno_val == ENOENT)
		status->exit_code = 127;
	else
		status->exit_code = 1;
	ft_putstr_fd(ERR_PREFIX, 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(errno_val), 2);
	ft_putstr_fd("\n", 2);
	
	return (status->exit_code);
}

