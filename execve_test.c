#include <unistd.h>

int main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;

    char *args[] = { "ls", NULL };
    execve("bin/ls", args, envp);
    return 0;
}
