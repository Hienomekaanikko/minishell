NAME	= minishell
LIBFT	= ./libft
HEADERS	= -I ./includes -I ${LIBFT}
LIBS	= ${LIBFT}/libft.a
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRC = main.c init_base.c init_loop.c lexer.c lexer_utils.c syntax_err.c ast_builder.c execution.c exec_pipe2.c execution_utils.c builtins.c builtins2.c exec_pipe.c envp.c arena.c arena_utils.c exec_redir.c builtin_export.c builtin_echo.c expansions.c ast_set_complex.c ast_set_complex_utils.c ast_set_complex2.c ast_utils.c ast_utils2.c var_list.c var_list_utils.c expansion_utils.c free_mem.c signals.c exec_error.c graphics.c find_executables.c here_doc.c parser.c parser_utils.c
OBJS = $(SRC:.c=.o)

all: $(NAME)

$(LIBS):
	$(MAKE) -C $(LIBFT)

%.o: %.c
	$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -lreadline -o $(NAME)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT) fclean

re: fclean all

debug: CFLAGS += -g -fsanitize=address -fsanitize=undefined -Wunused -Wunreachable-code
debug: all

.PHONY: all clean fclean re debug
