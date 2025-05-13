#include "minishell.h"

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

void check_path_permissions(char *path, t_exec_status *exec_status)
{
	struct stat path_stat;

	if ((path[0] == '.' && path[1] == '/') || path[0] == '/')
	{
		if (stat(path, &path_stat) == -1)
			exit(error_handler(exec_status, strerror(errno), 127));
		if (S_ISDIR(path_stat.st_mode))
			exit(error_handler(exec_status, "is a directory", 126));
		if (access(path, X_OK) == -1)
			exit(error_handler(exec_status, "Permission denied", 126));
		return;
	}
}

int	executables(t_ast *node, t_arena *env_arena, t_exec_status *exec_status)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
		return (error_handler(exec_status, "fork: failed", 1));
	if (pid == 0)
	{
		setup_child_signals();
		check_path_permissions(node->cmd, exec_status);
		path = find_executable(node, env_arena);
		if (!path)
			exit(error_handler(exec_status, "command not found", 127));
		execve(path, node->args, env_arena->ptrs);
		exit(error_handler(exec_status, "command not found", 127)); 
	}
	else if (pid > 0)
	{
		exec_status->pid = pid;
		wait_process(pid, exec_status);
	}
	return (0);
}

int	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	if (!node)
		return (error_handler(exec_status, "syntax error: invalid command", 1));
	//print_node_structure(node); //DEBUG
	if (node->type == RE_OUT || node->type == APPEND_OUT || node->type == RE_IN || node->type == HERE_DOC)
		return (exec_redir(node, env_arena, exec_status, exec_arena));
	else if (node->type == PIPE)
		return (exec_pipe(node, env_arena, exec_status, exec_arena));
	else if (built_ins(node, env_arena, exec_status) == -1)
	{
		if (executables(node, env_arena, exec_status) == -1)
			return (error_handler(exec_status, "command not found", 127));
	}
	return (0);
}
//
//echo <"./test_files/infile" <missing <"./test_files/infile"
