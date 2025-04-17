#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void exec_pipe(char **cmd1, char **cmd2) {
	printf("DEBUG: Starting pipe execution\n");
	printf("DEBUG: cmd1[0]: %s\n", cmd1[0]);
	printf("DEBUG: cmd2[0]: %s\n", cmd2[0]);
	
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		return;
	}
	printf("DEBUG: Pipe created successfully\n");

	pid_t pid1 = fork();
	if (pid1 == -1) {
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}

	if (pid1 == 0) {
		printf("DEBUG: Child process 1 (pid: %d) executing %s\n", getpid(), cmd1[0]);
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exec_single(cmd1);
		exit(EXIT_FAILURE);
	}

	pid_t pid2 = fork();
	if (pid2 == -1) {
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}

	if (pid2 == 0) {
		printf("DEBUG: Child process 2 (pid: %d) executing %s\n", getpid(), cmd2[0]);
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exec_single(cmd2);
		exit(EXIT_FAILURE);
	}

	printf("DEBUG: Parent process waiting for children\n");
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	printf("DEBUG: Pipe execution completed\n");
}

void exec_single(char **cmd) {
	printf("DEBUG: Starting single command execution\n");
	printf("DEBUG: Command: %s\n", cmd[0]);
	printf("DEBUG: Full command: ");
	for (int i = 0; cmd[i] != NULL; i++) {
		printf("%s ", cmd[i]);
	}
	printf("\n");

	if (strcmp(cmd[0], "cd") == 0) {
		printf("DEBUG: Handling cd command\n");
		if (cmd[1] == NULL) {
			chdir(getenv("HOME"));
		} else {
			if (chdir(cmd[1]) != 0) {
				perror("cd");
			}
		}
		return;
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return;
	}

	if (pid == 0) {
		printf("DEBUG: Child process (pid: %d) executing %s\n", getpid(), cmd[0]);
		execvp(cmd[0], cmd);
		printf("DEBUG: execvp failed for %s\n", cmd[0]);
		perror("execvp");
		exit(EXIT_FAILURE);
	} else {
		printf("DEBUG: Parent process waiting for child %d\n", pid);
		waitpid(pid, NULL, 0);
		printf("DEBUG: Single command execution completed\n");
	}
} 