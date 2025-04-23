/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 10:13:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/23 10:46:10 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_dollars(char *value)
{
	int	i;
	int	amount;

	i = 0;
	amount = 0;
	while (value[i])
	{
		if (value[i] == '$')
			amount++;
		i++;
	}
	return (amount);
}

void	extract_keys(t_exp_data *exp, char *value)
{
	int		i;
	int		j;
	int		current;
	int		start;

	i = 0;
	current = 0;
	start = 0;
	exp->dollars = count_dollars(value);
	if (exp->dollars)
	{
		exp->extracted_keys = malloc((exp->dollars + 1) * sizeof(char *));
		if (!exp->extracted_keys)
			return ;
		exp->values = malloc((exp->dollars + 1) * sizeof(char *));
		if (!exp->values)
			return ;
	}
	else
		return ;
	while (value[i])
	{
		if (value[i] == '$')
		{
			j = 0;
			i++;
			start = i;
			if (value[i] == '{')
			{
				i++;
				exp->brace_in = 1;
				j = i;
				while (value[j] && value[j] != 32 && value[j] != '$')
				{
					if (exp->brace_in == 1 && value[j] == '}')
					{
						exp->brace_out = 1;
						break;
					}
					j++;
				}
				exp->extracted_keys[current] = malloc((j - i + 1) * sizeof(char));
				ft_strlcpy(exp->extracted_keys[current], value + i, j - i + 1);
			}
			else
			{
				while(value[i] && value[i] != 32 && value[i] != '$' && value[i] != '"')
				{
					i++;
					j++;
				}
				i--;
				exp->extracted_keys[current] = ft_substr(value, start, j);
			}
			current++;
		}
		else
			i++;
	}
	exp->extracted_keys[current] = NULL;
}


void	init_exp(t_exp_data *exp)
{
	exp->brace_in = 0;
	exp->brace_out = 0;
	exp->key = NULL;
	exp->values = NULL;
	exp->extracted_keys = NULL;
	exp->dollars = 0;
}

int	count_expanded_size(char *value)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while(value[i])
	{
		if (value[i] == '$' && value[i + 1] == '{')
		{
			while (value[i] && value[i] != '}')
				i++;
		}
		i++;
		len++;
	}
	return (len);
}

char	*expand(char *value, char **expanded)
{
	char	*new_value;
	int		i;
	int		j;
	int		y;

	i = 0;
	j = 0;
	y = 0;
	printf("%d\n", new_value_len(value, expanded));
	new_value = ft_strdup("");
	while (value[i])
	{
		if (value[i] == '$')
		{
			while (value[i] && !ft_isspace(value[i + 1]))
				i++;
			if (*expanded)
			{
				new_value = ft_strjoin(new_value, *expanded);
				expanded++;
			}
			else
			{
				while (value[i] && value[i] != '$' && !ft_isspace(value[i]))
					i++;
				if (value[i] != '$')
					break;
				else
					i--;
			}
			j = ft_strlen(new_value);
		}
		else
		{
			new_value[j] = value[i];
			j++;
		}
		i++;
	}
	new_value[j] = '\0';
	return (new_value);
}

char	*cleanup(char *value)
{
	char	*new_value;
	int		i;

	i = 0;
	while(value[i] && value[i] != '$')
		i++;
	new_value = malloc((i + 1) * sizeof(char));
	i = 0;
	while(value[i] != '$')
	{
		new_value[i] = value[i];
		i++;
	}
	new_value[i] = '\0';
	free(value);
	return (new_value);
}

void	check_for_expansions(t_data *data)
{
	t_exp_data	exp;
	t_lexer		*temp;
	int			i;

	init_exp(&exp);
	temp = *data->lexed_list;
	while(temp)
	{
		if (temp->value[0] == '\'')
		{
			temp = temp->next;
			continue;
		}
		extract_keys(&exp, temp->value);
		if (exp.extracted_keys)
		{
			i = 0;
			while (exp.extracted_keys[i])
			{
				exp.values[i] = lookup(data->exp_map, exp.extracted_keys[i]);
				i++;
			}
			i = 0;
			if (exp.values[i])
				temp->value = expand(temp->value, exp.values);
			else
				temp->value = cleanup(temp->value);
		}
		temp = temp->next;
	}
}
