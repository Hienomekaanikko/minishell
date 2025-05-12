#include "minishell.h"

int	built_ins(t_ast *node, t_arena *env_arena, t_exec_status *status)
{
	if (ft_strncmp(node->cmd, "echo", 5) == 0)
		return (builtin_echo(node->args, status, env_arena));
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

int	executables(t_ast *node, t_arena *env_arena, t_exec_status *status)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == -1)
		return (handle_errno_error(status, "fork", errno));
	if (pid == 0)
	{
		setup_child_signals();
		path = find_executable(node, env_arena);
		if (!path)
		{
			check_file_status(node->cmd, status);
			exit(status->exit_code);
		}
		execve(path, node->args, env_arena->ptrs);
		free(path);
		exit(handle_errno_error(status, node->cmd, errno));
	}
	status->pid = pid;
	wait_process(pid, status);
	return (0);
}

// static void print_node_structure(t_ast *node) //DEBUG print the entire node structure
// {
//     printf("\nNode structure:\n");
//     printf("%d\n", node->type);
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

int	execute_command(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
	if (!node)
		return (error_handler(status, NULL, ERR_SYNTAX));
	//print_node_structure(node); //DEBUG
	if (node->type == RE_OUT || node->type == APPEND_OUT || node->type == RE_IN)
		return (exec_redir(node, env_arena, status, exec_arena));
	else if (node->type == HERE_DOC)
		return (exec_heredoc(node, env_arena, status, exec_arena));
	else if (node->type == PIPE)
		return (exec_pipe(node, env_arena, status, exec_arena));
	else if (built_ins(node, env_arena, status) == -1)
	{
		if (executables(node, env_arena, status) == -1)
			return (handle_errno_error(status, node->cmd, errno));
	}
	return (0);
}
