/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:27:06 by msuokas           #+#    #+#             */
/*   Updated: 2025/05/15 15:51:46 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdint.h>

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_toupper(int c);
int			ft_tolower(int c);
int			ft_isnum(char *num);
int			ft_isprint(int c);
int			ft_atoi(const char *str);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int			ft_strncmp(char *str1, char *str2, size_t n);
void		*ft_memset(void *dest, int x, size_t n);
void		ft_bzero(void *s, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putstr(char *str);
void		ft_putchar_fd(char c, int fd);
void		ft_free_split(char **args);
void		ft_putstr_fd(const char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		*ft_calloc(size_t count, size_t size);
void		*ft_memchr(const void *s, int c, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		*ft_itoa(int n);
char		*ft_strnstr(const char *haystack, const char *needle, size_t n);
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
size_t		ft_strlcpy(char *dest, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
size_t		ft_strlen(const char *str);
long long	ft_atol(const char *s);
int			ft_isspace(int c);
char		*ft_strndup(const char *s, size_t n);
void		ft_free_substrings(char **arr_str);
char		*ft_strjoin_free(char *s1, char *s2);
int			is_space(char *cmd);
char		**ft_special_split(char const *s, char c);
char		**ft_no_quotes_split(char const *s, char c);
#endif
