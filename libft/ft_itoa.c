/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 12:20:48 by msuokas           #+#    #+#             */
/*   Updated: 2024/11/20 09:38:37 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_length(int n)
{
	int	i;

	i = 0;
	if (n <= 0)
		i++;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static int	check_if_neg(int n)
{
	if (n < 0)
		n = -n;
	return (n);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		str_length;
	int		i;

	str_length = count_length(n);
	i = str_length;
	str = malloc((str_length + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[str_length] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		i--;
	}
	while (str_length-- && i > 0)
	{
		str[str_length] = check_if_neg(n) % 10 + '0';
		n /= 10;
		i--;
	}
	return (str);
}
