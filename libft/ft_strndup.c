/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:36:51 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 12:37:00 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(s);
	new_str = (char *)ft_calloc(len + 1, sizeof(char));
	if (new_str)
	{
		ft_memcpy(new_str, s, n);
	}
	return (new_str);
}
