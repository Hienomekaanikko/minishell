/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_substrings.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:10:11 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/10 16:10:21 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_substrings(char **arr_str)
{
	int	i;

	i = 0;
	while (arr_str[i] != NULL)
	{
		free(arr_str[i]);
		arr_str[i] = NULL;
		i++;
	}
	if (arr_str != NULL)
	{
		free(arr_str);
		arr_str = NULL;
	}
}
