#include "minishell.h"

int exec_heredoc(t_ast *node, t_arena *env_arena, t_exec_status *status, t_arena *exec_arena)
{
    (void)env_arena;
    (void)status;

	handle_heredoc_signals();
    printf("DEBUG: Node type: %d\n", node->type);
    printf("DEBUG: Node cmd: |%s|\n", node->cmd ? node->cmd : "NULL");
    printf("DEBUG: Node file: |%s|\n", node->file ? node->file : "NULL");
    if (node->args)
    {
        printf("DEBUG: Node args:\n");
        for (int i = 0; node->args[i]; i++)
            printf("DEBUG:  [%d]: |%s|\n", i, node->args[i]);
    }
    if (node->right)
    {
        printf("DEBUG: Right node cmd: |%s|\n", node->right->cmd ? node->right->cmd : "NULL");
        printf("DEBUG: Right node file: |%s|\n", node->right->file ? node->right->file : "NULL");
    }
    // DEBOG print arena content
    printf("\nDEBUG: Arena content:\n");
    for (size_t i = 0; i < exec_arena->ptrs_in_use; i++)
    {
        printf("DEBUG: Arena[%zu]: |%s|\n", i, exec_arena->ptrs[i]);
    }
    arena_add(exec_arena, "DEBUG: test heredoc content");
    return (0);
}