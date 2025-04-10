/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:22:34 by msuokas           #+#    #+#             */
/*   Updated: 2024/11/18 11:25:21 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new_str;
	int		trim_start;
	int		new_len;

	if (!s1 || !set)
		return (NULL);
	trim_start = 0;
	new_len = ft_strlen(s1) - 1;
	while (s1[trim_start] && ft_strchr(set, s1[trim_start]))
		trim_start++;
	while (new_len > trim_start && ft_strchr(set, s1[new_len]))
		new_len--;
	new_len = new_len - trim_start + 1;
	new_str = ft_calloc(1 + new_len, sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_memcpy(new_str, &s1[trim_start], new_len);
	new_str[new_len] = '\0';
	return (new_str);
}
