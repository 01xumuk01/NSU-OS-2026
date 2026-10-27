#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

int execvpe(const char *file, char *argv[], char *envp[]) {

	char **old_env = environ;
	environ = envp;
	int result = execvp(file, argv);
	environ = old_env;

	return result;
}

int main(int argc, char* args[]) {

        if (argc < 2) {
                printf("Usage: %s command [arguments...]\n", args[0]);
                return 1;
        }

        pid_t pid = fork();

        if (pid == -1) {
                perror("Error when creating a child process");
                return 1;
        } else if (pid == 0) {
		char *new_env[] = {
        		"PATH=/usr/bin:/bin",
        		"LAB=11",
        		NULL
		};

                execvpe(args[1], &args[1], new_env);
                perror("Error after calling execvpe");
                return 1;
        } else {
                int status;

                if (waitpid(pid, &status, 0) == -1) {
                        perror("waitpid");
                        return 1;
                }

                if (WIFEXITED(status)) {
                        printf("The child exit code: %d\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                        printf("The child terminated by signal: %d\n", WTERMSIG(status));
                }
        }

        return 0;
}
