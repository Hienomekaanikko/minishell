
# include "minishell.h"

t_arena	*init_env_arena(char **envp)
{
	t_arena	*env_arena;
	size_t	env_count;
	size_t	i;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	env_arena = arena_init(env_count * 100 * 2, env_count + 32);
	if (!env_arena)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		arena_add(env_arena, envp[i]);
		i++;
	}
	return (env_arena);
}
