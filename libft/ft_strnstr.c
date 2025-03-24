/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:22:42 by msuokas           #+#    #+#             */
/*   Updated: 2024/11/18 11:24:30 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	y;

	i = 0;
	if (needle[0] == 0)
		return ((char *)&haystack[i]);
	if (len == 0)
		return (NULL);
	while (haystack[i] != '\0' && i < len)
	{
		y = 0;
		while (haystack[i + y] == needle[y]
			&& haystack[i + y] != '\0' && (i + y) < len)
		{
			if (needle[y + 1] == '\0')
				return ((char *)&haystack[i]);
			y++;
		}
		i++;
	}
	return (NULL);
}
