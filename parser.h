/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:19:25 by msuokas           #+#    #+#             */
/*   Updated: 2025/06/10 13:17:21 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "minishell.h"

typedef struct s_parser
{
	int		i;
	int		y;
	int		start;
	int		in_quote;
	int		in;
	char	quote;
	char	op;
}	t_parser;

typedef struct s_counter
{
	int		count;
	int		in_word;
	char	op;
	char	op_len;
}	t_counter;

#endif
