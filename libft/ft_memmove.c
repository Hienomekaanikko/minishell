/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:49:38 by msuokas           #+#    #+#             */
/*   Updated: 2024/11/20 09:41:24 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*to;
	unsigned char	*from;

	if (!dest && !src)
		return (NULL);
	to = (unsigned char *)dest;
	from = (unsigned char *)src;
	if (from < to && to < from + n)
		while (n--)
			to[n] = from[n];
	else
		while (n--)
			*(to++) = *(from++);
	return (dest);
}
