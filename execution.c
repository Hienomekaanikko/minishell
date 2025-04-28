#include "minishell.h"

char *try_path(char *cmd, char *path_str)
{
	char	**paths;
	char	*path_to_executable;
	int		i;

	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_to_executable = ft_strjoin(paths[i], "/");
		path_to_executable = ft_strjoin(path_to_executable, cmd);
		if (access(path_to_executable, X_OK) == 0)
		{
			ft_free_split(paths);
			return path_to_executable;
		}
		free(path_to_executable);
		i++;
	}
	ft_free_split(paths);
	return (NULL);
}

void	handle_exec_error(t_exec_status *exec_status, int status, char *error_msg, int exit_code)
{
	if (error_msg)
	{
		exec_status->signal = 0;
		exec_status->error_msg = error_msg;
		exec_status->exit_code = exit_code;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(error_msg, 2);
		ft_putstr_fd("\n", 2);
	}
	else if (WIFSIGNALED(status))
	{
		exec_status->signal = WTERMSIG(status);
		exec_status->exit_code = 128 + exec_status->signal;
		if (exec_status->signal == SIGSEGV)
			exec_status->error_msg = "Segmentation fault";
		else if (exec_status->signal == SIGBUS)
			exec_status->error_msg = "Bus error";
		else if (exec_status->signal == SIGFPE)
			exec_status->error_msg = "Floating point exception";
		else if (exec_status->signal == SIGILL)
			exec_status->error_msg = "Illegal instruction";
		else if (exec_status->signal == SIGTERM)
			exec_status->error_msg = "Terminated";
		else if (exec_status->signal == SIGKILL)
			exec_status->error_msg = "Killed";
		else
			exec_status->error_msg = "Unknown signal";
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(exec_status->error_msg, 2);
		ft_putstr_fd("\n", 2);
	}
	else
		exec_status->exit_code = WEXITSTATUS(status);
}

char *find_executable(t_ast *node, t_arena *env_arena)
{
    char *executable;

    executable = try_path(node->cmd, SECURE_PATH);
    if (executable)
        return (executable);
	printf("Command not found in secure path. Searching through environment.\n");
	executable = try_path(node->cmd, arena_getenv(env_arena, "PATH"));
	return (executable);
}

int	built_ins(t_ast *node, t_arena *env_arena, t_exec_status *status)
{ 
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, status));
	else if (ft_strncmp(node->cmd, "cd", 3) == 0)
		return (builtin_cd(node->args));
	else if (ft_strncmp(node->cmd, "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(node->cmd, "export", 7) == 0)
		return (builtin_export());
	else if (ft_strncmp(node->cmd, "unset", 6) == 0)
		return (builtin_unset());
	else if (ft_strncmp(node->cmd, "env", 4) == 0)
		return (builtin_env(env_arena));
	return (-1);
}

int	executables(t_ast *node, t_arena *env_arena, t_exec_status *exec_status)
{
	pid_t	pid;
	char	*path;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		handle_exec_error(exec_status, 0, "Fork failed", 1);
		return (1);
	}
	if (pid == 0)
	{
		path = find_executable(node, env_arena);
		if (!path)
			exit(127);
		execve(path, node->args, env_arena->ptrs);
		free(path);
		exit(126);
	}
	else if (pid > 0)
	{
		exec_status->pid = pid;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			exec_status->exit_code = WEXITSTATUS(status);
			if (exec_status->exit_code == 127)
				handle_exec_error(exec_status, 0, "Command not found", 127);
			else if (exec_status->exit_code == 126)
				handle_exec_error(exec_status, 0, "Permission denied", 126);
		}
		else if (WIFSIGNALED(status))
			handle_exec_error(exec_status, status, NULL, 0);
	}
	return (0);
}

void	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	if (node->type == PIPE)
	{
		exec_pipe(node, env_arena, exec_status, exec_arena);
		return ;
	}
	if (node->type == RE_OUT)
	{
		exec_re_out(node, env_arena, exec_status, exec_arena);
		return ;
	}
	if (built_ins(node, env_arena, exec_status) != -1)
		return ;
	if(executables(node, env_arena, exec_status) == -1)
		ft_putstr_fd("command not found\n", 2);
}