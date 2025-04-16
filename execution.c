#include "minishell.h"

void	debug_print(char *msg)  //DEBUG
{
	ft_putstr_fd("\033[31mDEBUG: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\033[0m \n", 2);
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
	printf("Command not found in secure path. Searching through environment. Risky, right?\n");
	executable = try_path(node->cmd, getenv("PATH"));
	return (executable);
}

int	built_ins(t_ast *node, t_exec_status *status)
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
		return (builtin_env());
	return (-1);
}

int	executables(t_ast *node, char **env, t_exec_status *exec_status)
{
	pid_t	pid;
	char	*path;
	int		status;
	
	node->env = env;
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Fork failed\n", 2);
		return (1);
	}
	if (pid == 0)
	{
		path = find_executable(node);
		if(!path)
		{
			ft_putstr_fd("Command not found\n", 2);
			exit(127);
		}	
		execve(path,node->args, node->env);
		free(path);
		ft_putstr_fd("Permission denied\n", 2);
		exit(1);
	}
	else if (pid > 0)
	{
		exec_status->pid = pid;
		waitpid(pid, &status, 0);
		if(WIFSIGNALED(status))
		// TODO: signal message to words here: SIGSEGV -> "Segmentation fault" etc
			exec_status->exit_code = 128 + WTERMSIG(status);
		else
			exec_status->exit_code = WEXITSTATUS(status);
	}
	return (0);
}

void	execute_command(t_ast *node, char **env, t_exec_status *exec_status)
{
	node->env = env;
	if (built_ins(node, exec_status) != -1)
		return ;
	if(executables(node, env, exec_status) == -1)
		ft_putstr_fd("command not found\n", 2);
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