#include "minishell.h"

//just a simple main to get building stuff!

int main(int argc, char **argv, char **envp)
{
	char	*input;
	t_data	data;

	(void)argc; //maybe something later
	(void)argv; //maybe something later
	(void)envp; //need this later for finding command paths etc.
	while (1)
	{
		input = readline("minishell$ ");
		if (input == NULL)
			break;
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break;
		}
		data.input = &input;

		// replace this with the lexing, parsing etc.
		printf("input: %s\n", *data.input);
		free(input);
	}
	return (0);
}
