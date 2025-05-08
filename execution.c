#include "minishell.h"

int	built_ins(t_ast *node, t_arena *env_arena, t_exec_status *status)
{
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, status, env_arena));
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

	pid = fork();
	if (pid == -1)
		return (error_handler(exec_status, "fork: failed", 1));
	if (pid == 0)
	{
		setup_child_signals();
		path = find_executable(node, env_arena);
		if (!path)
			exit(error_handler(exec_status, "command not found", 127));
		execve(path, node->args, env_arena->ptrs);
		free(path);
		exit(error_handler(exec_status, "permission denied", 126));
	}
	else if (pid > 0)
	{
		exec_status->pid = pid;
		wait_process(pid, exec_status);
	}
	return (0);
}

// static void print_node_structure(t_ast *node) //DEBUG print the entire node structure
// {
//     printf("\nNode structure:\n");
//     printf("Type: %d\n", node->type);
//     printf("Cmd: |%s|\n", node->cmd ? node->cmd : "NULL");
//     printf("File: |%s|\n", node->file ? node->file : "NULL");
//     if (node->args)
//     {
//         printf("Args:\n");
//         for (int i = 0; node->args[i]; i++)
//             printf("  [%d]: |%s|\n", i, node->args[i]);
//     }
//     if (node->left)
//     {
//         printf("Left child:\n");
//         printf("  Type: %d\n", node->left->type);
//         printf("  Cmd: |%s|\n", node->left->cmd ? node->left->cmd : "NULL");
//     }
//     if (node->right)
//     {
//         printf("Right child:\n");
//         printf("  Type: %d\n", node->right->type);
//         printf("  Cmd: |%s|\n", node->right->cmd ? node->right->cmd : "NULL");
//     }
//     printf("\n");
// }

int	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *exec_status, t_arena *exec_arena)
{
	if (!node)
		return (error_handler(exec_status, "syntax error: invalid command", 1));
	//print_node_structure(node); //DEBUG
	if (node->type == PIPE)
		return (exec_pipe(node, env_arena, exec_status, exec_arena));
	else if (node->type == RE_OUT || node->type == APPEND_OUT || node->type == RE_IN)
		return (exec_redir(node, env_arena, exec_status, exec_arena));
	else if (node->type == HERE_DOC)
		return (exec_heredoc(node, env_arena, exec_status, exec_arena));
	else if (built_ins(node, env_arena, exec_status) == -1)
	{
		if (executables(node, env_arena, exec_status) == -1)
			return (error_handler(exec_status, "command not found", 127));
	}
	return (0);
}