
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

char	*arena_getenv(t_arena *env_arena, char *key)
{
	size_t	i;
	size_t	key_len;
	char	*env_var;

	if(!env_arena || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while(i < env_arena->ptrs_in_use)
	{
		env_var = env_arena->ptrs[i];
		if (ft_strncmp(env_var, key, key_len) == 0 && env_var[key_len] == '=')
			return (env_var + key_len + 1);
		i++;
	}
	return (NULL);
}
