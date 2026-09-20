#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Child process PID: %d\n", (int)getpid());
    }
    else {
        printf("Parent process PID: %d\n", (int)getpid());
    }

    return 0;
}

