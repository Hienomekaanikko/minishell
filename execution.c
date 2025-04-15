#include "minishell.h"

void	debug_print(char *msg)
{
	ft_putstr_fd("\033[31mDEBUG: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\033[0m \n", 2);
}

void	error_command_not_found(t_ast *node)
{
	ft_putstr_fd("Minishell: Command not found: ", 2);
	ft_putstr_fd(node->cmd, 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

void	error_execve_failed(char *path)
{
	(void) path;
	ft_putstr_fd("Execve failed\n", 2);
	free(path);
	exit(1);
}

char *try_path(char *cmd, char *path_str)
{
	char **paths;
	char *path_to_executable;
	int i;

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

char *find_executable(t_ast *node)
{
    char *executable;

    executable = try_path(node->cmd, SECURE_PATH);
    if (executable)
        return (executable);
	printf("Command not found in secure path. Searching through environment. Risky, right?");
	executable = try_path(node->cmd, getenv("PATH"));
   if (executable)
		return (executable);
	return (NULL);
}

int	built_ins(t_ast *node, t_exec_status *status)
{ 
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, status));
	else if (ft_strncmp(node->cmd, "cd", 3) == 0)
		return (builtin_cd());
	else if (ft_strncmp(node->cmd, "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(node->cmd, "export", 7) == 0)
		return (builtin_export());
	else if (ft_strncmp(node->cmd, "unset", 6) == 0)
		return (builtin_unset());
	else if (ft_strncmp(node->cmd, "env", 4) == 0)
		return (builtin_env());
	return (-1);
}

int	executables(t_ast *node, char **env, t_exec_status *exec_status)
{
	pid_t	pid;
	char	*path;
	int		status;
	
	node->env = env;
	debug_print("After built-ins, before fork.");
	pid = fork();
	if (pid == 0)
	{
		debug_print("In child process");
		path = find_executable(node);
		printf("Path found: %s\n", path);
		if(!path)
			error_command_not_found(node);
		execve(path,node->args, node->env);
		error_execve_failed(path);
	}
	else if (pid > 0)
	{
		debug_print("In parent. Waiting for child.");
		waitpid(pid, &status, 0);
		debug_print("Child finished.");
		exec_status->exit_code = WEXITSTATUS(status);
	}
	return (0);
}

void	execute_command(t_ast *node, char **env, t_exec_status *exec_status)
{
	node->env = env;
	if (built_ins(node, exec_status) == -1)
		if(executables(node, env, exec_status) == -1)
			printf("command not found");
}


// int	main(int ac, char **av, char **env)
// {
// 	if (ac < 2)
// 	{
// 		ft_putstr_fd("Usage: ./minishell <command> [args]\n", 2);
// 		return (1);
// 	}

// 	t_ast cmd_node = {
// 		.type = CMD,
// 		.cmd = av[1],
// 		.args = &av[1],
// 		.env = env,
// 		.infile = NULL,
// 		.outfile = NULL,
// 		.left = NULL,
// 		.right = NULL,
// 	};
// 	execute_command(&cmd_node);
// 	return (0);
// }