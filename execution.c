#include "minishell.h"

char *try_path(char *cmd, char *path_str, t_exec_status *status)
{
	char	**paths;
	char	*path_to_executable;
	int		i;

	if (!path_str)
		return (handle_exec_error(status, "PATH not set", 127));
	paths = ft_split(path_str, ':');
	if (!paths)
		return (handle_exec_error(status, "Memory allocation failed", 1));
	i = 0;
	while (paths[i])
	{
		path_to_executable = ft_strjoin(paths[i], "/");
		if (!path_to_executable)
		{
			ft_free_split(paths);
			return (handle_exec_error(status, "Memory allocation failed", 1));
		}
		char *temp = ft_strjoin(path_to_executable, cmd);
		free(path_to_executable);
		if (!temp)
		{
			ft_free_split(paths);
			return (handle_exec_error(status, "Memory allocation failed", 1));
		}
		path_to_executable = temp;
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


char *find_executable(t_ast *node, t_arena *env_arena)
{
    char *executable;

    executable = try_path(node->cmd, SECURE_PATH, NULL);
    if (executable)
        return (executable);
	executable = try_path(node->cmd, arena_getenv(env_arena, "PATH"), NULL);
	return (executable);
}

int	built_ins(t_ast *node, t_arena *env_arena, t_exec_status *status)
{ 
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, status));
	else if (ft_strncmp(node->cmd, "cd", 3) == 0)
		return (builtin_cd(node->args, status));
	else if (ft_strncmp(node->cmd, "pwd", 4) == 0)
		return (builtin_pwd(status));
	else if (ft_strncmp(node->cmd, "export", 7) == 0)
		return (builtin_export(env_arena, status, node->args));
	else if (ft_strncmp(node->cmd, "unset", 6) == 0)
		return (builtin_unset(env_arena, status, node->args));
	else if (ft_strncmp(node->cmd, "env", 4) == 0)
		return (builtin_env(env_arena, status));
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
		handle_exec_error(exec_status, "Fork failed", 1);
		return (1);
	}
	if (pid == 0)
	{
		setup_child_signals();
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
			handle_exit_error(exec_status, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			handle_signal_error(exec_status, WTERMSIG(status));
	}
	return (0);
}

void	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	if (!node)
	{
		handle_exec_error(exec_status, "Invalid command", 1);
		return;
	}
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
	if (executables(node, env_arena, exec_status) == -1)
		handle_exec_error(exec_status, "Command not found", 127);
}