#include "minishell.h"

void	set_variable(t_data *data, char *key, char *value)
{
	if (arena_has_key(data->env_arena, key))
		arena_set_env(data, key, value);
	if (!already_declared(data->exp->var_list, key, value))
	{
		if (!add_var_to_list(data->exp, key, value))
			data->mem_error = 1;
	}
}

void	unset_local(t_var **head, char *key)
{
	t_var	*current;
	t_var	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
	return ;
}
