
#include "minishell.h"

static int	export_append(t_data *data, const char *key, char *value)
{
	char	*old_value;
	char	*new_value;

	old_value = is_declared(data, (char *)key);
	if (data->mem_error == 1)
		return (0);
	if (!old_value)
		new_value = ft_strdup(value);
	else
	{
		new_value = ft_strjoin(old_value, value);
		free(old_value);
	}
	if (!new_value)
	{
		data->mem_error = 1;
		return (0);
	}
	arena_set_env(data, (char *)key, new_value);
	free(new_value);
	return (0); // TODO: should some of these be non-zero?
}