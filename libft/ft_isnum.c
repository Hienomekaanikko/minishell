/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:10:20 by msuokas           #+#    #+#             */
/*   Updated: 2025/02/14 14:25:21 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isnum(char *num)
{
	int	i;

	i = 0;
	while (num[i] && ft_isspace(num[i]))
		i++;
	if (num[i] == '-' || num[i] == '+')
		i++;
	if (!ft_isdigit(num[i]))
		return (0);
	while (num[i] && ft_isdigit(num[i]))
		i++;
	while (num[i])
	{
		if (!ft_isspace(num[i]))
			return (0);
		i++;
	}
	return (1);
}
