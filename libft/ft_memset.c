/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:37:08 by msuokas           #+#    #+#             */
/*   Updated: 2024/11/18 10:46:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int x, size_t n)
{
	unsigned char	*temp;

	temp = (unsigned char *)dest;
	while (n > 0)
	{
		*temp = (unsigned char)x;
		n--;
		temp++;
	}
	return (dest);
}
