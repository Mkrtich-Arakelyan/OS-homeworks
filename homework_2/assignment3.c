#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process
        execl("/bin/echo",
              "echo",
              "Hello from the child process",
              NULL);

        perror("execl failed");
        return 1;
    }
    else {
        // Parent waits for echo to finish
        wait(NULL);

        printf("Parent process done\n");
    }

    return 0;
}
