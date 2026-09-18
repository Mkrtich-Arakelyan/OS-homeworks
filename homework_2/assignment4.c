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
        execl("/usr/bin/grep",
              "grep",
              "main",
              "test.txt",
              NULL);

        perror("execl failed");
        return 1;
    }
    else {
        // Parent process
        wait(NULL);

        printf("Parent process completed\n");
    }

    return 0;
}
