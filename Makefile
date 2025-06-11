NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RLFLAGS = -lreadline
OBJDIR = object_files
HEADERDIR = headers
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a
HEADERS = -I $(HEADERDIR) -I $(LIBFTDIR)

SRCS = \
	main_inits_cleanup/main.c \
	main_inits_cleanup/init_base.c \
	main_inits_cleanup/init_loop.c \
	main_inits_cleanup/free_mem.c \
	parsing/parser.c \
	parsing/parser_utils.c \
	parsing/parser_utils2.c \
	parsing/lexer.c \
	parsing/lexer_utils.c \
	parsing/lexer_utils2.c \
	parsing/syntax_err.c \
	ast_tree/ast_builder.c \
	ast_tree/ast_set_complex.c \
	ast_tree/ast_set_complex_utils.c \
	ast_tree/ast_set_complex2.c \
	ast_tree/ast_utils.c \
	ast_tree/ast_utils2.c \
	ast_tree/here_doc.c \
	ast_tree/ast_errors.c \
	ast_tree/here_doc_utils.c \
	execution/execution.c \
	execution/exec_pipe2.c \
	execution/execution_utils.c \
	execution/builtins.c \
	execution/builtins2.c \
	execution/exec_pipe.c \
	execution/exec_redir.c \
	execution/builtin_export.c \
	execution/builtin_echo.c \
	execution/find_executables.c \
	execution/exec_error.c \
	execution/builtin_export_append.c \
	data_handling/envp.c \
	data_handling/envp_utils.c \
	data_handling/arena.c \
	data_handling/arena_utils.c \
	data_handling/var_list.c \
	data_handling/var_list_utils.c \
	expansions/expansions.c \
	expansions/expansions2.c \
	expansions/expander2.c \
	expansions/expander.c \
	expansions/expansion_utils.c \
	utils_and_signals/signals.c \
	utils_and_signals/graphics.c \
	utils_and_signals/rl_handler.c \

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(RLFLAGS)

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFTDIR) clean
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

debug: CFLAGS += -g -fsanitize=address -fsanitize=undefined -Wunused -Wunreachable-code
debug: all

.PHONY: all clean fclean re debug
