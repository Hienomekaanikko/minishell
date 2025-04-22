#include "minishell.h"

size_t	env_var_count(char **env)
{
	size_t	count;

	count = 0;
	while(env[count])
		count++;
	return (count);
}

void	env_init(t_env *env, char **parent_env)
{
	size_t	i;

	//TODO: input validation
	env->capacity = env_var_count(parent_env);
	env->envp = malloc(sizeof(char*) * (env->capacity +1 ));
	if (!env->envp)
		return ; //TODO: error exit?
	i = 0;
	while(i < env->capacity)
	{
		env->envp[i] = ft_strdup(parent_env[i]); //TODO: check for strdup success and free previously allocated strings
		i++;
	}
	env->envp[env->capacity] = NULL;
	env->var_count = env->capacity;
}
void	env_add_space(t_env *env)
{
	char	**new_envp;
	char	**old_envp;
	size_t	i;

	i = 0;
	old_envp = env->envp;
	new_envp = malloc(sizeof(char *) * (env->capacity * 2 + 1));
	while(env->envp[i])
	{
		new_envp[i] = ft_strdup(env->envp[i]); //TODO: error free
		i++;
	}
	new_envp[i] = NULL;
	env->envp = new_envp;
	ft_free_substrings(old_envp);
	env->capacity *= 2;
}

void	env_add(t_env *env, char *key, char *value)
{
	char	*new_var;

	//TODO: input validation?
	if (env-> var_count + 1 >= env->capacity)
		env_add_space(env);
	new_var = ft_strjoin_free(key, "=");
	new_var = ft_strjoin_free(new_var, value);
	env->envp[env->var_count] = new_var;
	env->var_count++;
	env->envp[env->var_count] = NULL;
}

void	env_free(t_env *env)
{
	size_t	i;

	if (!env || !env->var_count)
		return ;
	ft_free_substrings(env->envp);
	env->envp = NULL;
	env->var_count = 0;
	env->capacity = 0;
}