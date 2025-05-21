/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonsdor <mbonsdor@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:48:40 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/21 18:08:45 by mbonsdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

typedef struct s_ast_utils
{
	int		i;
	int		j;
	int		len;
	int		quote;
	int		argument_amount;
	char	*cleaned_value;
}	t_ast_utils;
