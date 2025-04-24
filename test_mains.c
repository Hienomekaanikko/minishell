// void	debug_print(char *msg)  //DEBUG
// {
// 	ft_putstr_fd("\033[31mDEBUG: ", 2);
// 	ft_putstr_fd(msg, 2);
// 	ft_putstr_fd("\n", 2);
// 	ft_putnbr_fd(getpid(), 2);
// 	ft_putstr_fd("\033[0m \n", 2);
// }

// //execution

// // int	main(int ac, char **av, char **env)
// // {
// // 	if (ac < 2)
// // 	{
// // 		ft_putstr_fd("Usage: ./minishell <command> [args]\n", 2);
// // 		return (1);
// // 	}

// // 	t_ast cmd_node = {
// // 		.type = CMD,
// // 		.cmd = av[1],
// // 		.args = &av[1],
// // 		.env = env,
// // 		.infile = NULL,
// // 		.outfile = NULL,
// // 		.left = NULL,
// // 		.right = NULL,
// // 	};
// // 	execute_command(&cmd_node);
// // 	return (0);
// // }

// // arena
// int	main(void)
// {
// 	t_arena	*arena;
// 	arena = arena_init(128, 2);
// 	if (!arena)
// 	{
// 		printf("Failed to initialize arena\n");
// 		return (1);
// 	}

// 	// Add some strings to the arena
// 	arena_add(arena, "Hello");
// 	arena_add(arena, "World");
// 	arena_add(arena, "Test");

// 	// Print the strings directly from arena memory
// 	printf("str1: %s\n", arena->ptrs[0]);
// 	printf("str2: %s\n", arena->ptrs[1]);
// 	printf("str3: %s\n", arena->ptrs[2]);

// 	// Print arena stats
// 	printf("\nArena stats:\n");
// 	printf("Memory size: %zu\n", arena->mem_size);
// 	printf("Memory used: %zu\n", arena->mem_used);
// 	printf("Pointers in use: %zu\n", arena->ptrs_in_use);
// 	printf("Pointer capacity: %zu\n", arena->ptr_capacity);

// 	// Free the arena
// 	arena_free(arena);
// 	return (0);
// } 