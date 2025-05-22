#include "minishell.h"

int	built_ins(t_ast *node, t_arena *env_arena, t_exec_status *status)
{
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, status));
	else if (ft_strncmp(node->cmd, "cd", 3) == 0)
		return (builtin_cd(node->args, status, env_arena));
	else if (ft_strncmp(node->cmd, "pwd", 4) == 0)
		return (builtin_pwd(status, env_arena));
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
			exit(error_handler(exec_status, path, strerror(errno), 127));
		if (S_ISDIR(path_stat.st_mode))
			exit(error_handler(exec_status, path, "Is a directory", 126));
		if (access(path, X_OK) == -1)
			exit(error_handler(exec_status, path, "Permission denied", 126));
		return;
	}
}

void	close_fds(t_exec_status *exec_status)
{
	if (exec_status->saved_stdout != -1)
	{
		close(exec_status->saved_stdout);
		exec_status->saved_stdout = -1;
	}
	if (exec_status->temp_fd != -1)
	{
		close(exec_status->temp_fd);
		exec_status->temp_fd = -1;
	}
}

int	executables(t_ast *node, t_arena *env_arena, t_exec_status *exec_status)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
		return (error_handler(exec_status, "fork", "failed", 1));
	if (pid == 0)
	{
		setup_child_signals();
		check_path_permissions(node->cmd, exec_status);
		path = find_executable(node, env_arena);
		if (!path)
			exit(error_handler(exec_status, node->cmd, "command not found", 127));
		if (exec_status->outfile != -1)
		{
			if (dup2(exec_status->outfile, 1) == -1)
				exit(handle_redirection_error(exec_status->outfile, exec_status));
			close(exec_status->outfile);
			exec_status->outfile = -1;
		}
		if (exec_status->infile != -1)
		{
			if (dup2(exec_status->infile, 0) == -1)
				exit(handle_redirection_error(exec_status->infile, exec_status));
			close(exec_status->infile);
			exec_status->infile = -1;
		}
		close_fds(exec_status);
		execve(path, node->args, env_arena->ptrs);
		free(path);
		exit(error_handler(exec_status, node->cmd, "command not found", 127));
	}
	else
	{
		exec_status->pid = pid;
		wait_process(pid, exec_status);
	}
	return (0);
}

int	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status)
{
	if (!node)
		return (error_handler(exec_status, node->cmd, "syntax error: invalid command", 1));
	if (node->type == RE_OUT || node->type == APPEND_OUT || node->type == RE_IN || node->type == HERE_DOC)
		return (exec_redir(node, env_arena, exec_status));
	else if (node->type == PIPE && exec_status->redir_fail == 0)
		return (exec_pipe(node, env_arena, exec_status));
	else
	{
		if (exec_status->outfile != -1)
		{
			exec_status->saved_stdout = dup(STDOUT_FILENO);
			dup2(exec_status->outfile, STDOUT_FILENO);
		}
		if (built_ins(node, env_arena, exec_status) == -1)
		{
			if (exec_status->redir_fail == 0)
			{
				if (executables(node, env_arena, exec_status) == -1)
					return (error_handler(exec_status, node->cmd, "command not found", 127));

			}
		}
		if (exec_status->outfile != -1 && exec_status->saved_stdout != -1)
		{
			dup2(exec_status->saved_stdout, STDOUT_FILENO);
			close(exec_status->saved_stdout);
			close(exec_status->outfile);
			if (exec_status->temp_fd != -1)
				close(exec_status->temp_fd);
		}
	}
	if (exec_status->redir_fail == 1)
		exec_status->exit_code = 1;
	return (0);
}
