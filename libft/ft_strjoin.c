/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:47:08 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/17 14:17:50 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *dest, char *src)
{
	size_t	dest_len;
	size_t	src_len;
	char	*new_str;

	dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	new_str = malloc((dest_len + src_len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	ft_memcpy(new_str, dest, dest_len);
	ft_memcpy(new_str + dest_len, src, src_len + 1);
	return (new_str);
}
