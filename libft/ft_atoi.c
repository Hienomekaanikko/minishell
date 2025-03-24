/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:31:44 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/17 17:55:57 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static const char	*skip_spaces(const char *str)
{
	int	i;

	i = 0;
	while ((str[i] >= 8 && str[i] <= 13) || str[i] == 32)
		i++;
	return (&str[i]);
}

int	ft_atoi(const char *str)
{
	long	result;
	int		i;
	long	sign;

	i = 0;
	result = 0;
	sign = 1;
	str = skip_spaces(str);
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i++] - '0');
		if (result < 0 && sign == 1)
			return ((int)LONG_MAX);
		if (result < 0 && sign == -1)
			return ((int)LONG_MIN);
	}
	return ((int)(result * sign));
}
