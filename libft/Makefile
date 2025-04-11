NAME = libft.a

SRC = ft_calloc.c \
			ft_atol.c \
			ft_isspace.c \
			ft_isnum.c \
			ft_strlen.c \
			ft_strdup.c \
			ft_memset.c \
			ft_bzero.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_strchr.c \
			ft_strrchr.c \
			ft_strnstr.c \
			ft_strncmp.c \
			ft_atoi.c \
			ft_isdigit.c \
			ft_isalpha.c \
			ft_isalnum.c \
			ft_isascii.c \
			ft_isprint.c \
			ft_toupper.c \
			ft_tolower.c \
			ft_striteri.c \
			ft_strmapi.c \
			ft_substr.c \
			ft_strjoin.c \
			ft_strtrim.c \
			ft_putendl_fd.c \
			ft_split.c \
			ft_putstr.c \
			ft_free_split.c \
			ft_putstr_fd.c \
			ft_strndup.c \
			ft_free_substrings.c \
			ft_strjoin_free.c \
			ft_is_space.c \
			ft_special_split.c

OBJS = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -rf
AR = ar crs

$(NAME): $(OBJS)
	$(AR) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
