/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:19:25 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/26 14:12:49 by msuokas          ###   ########.fr       */
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

void	add_operator(t_parser *data, const char *s);
int		make_substring(t_parser *data, char const *s, char **array_of_strings);
char	**free_malloc(char **array_of_strings, int y);
void	skip_word(t_counter *data, const char *s);

#endif
