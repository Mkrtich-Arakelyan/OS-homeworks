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
        printf("Child PID: %d\n", (int)getpid());
        return 0;
    }
    else {
        printf("Parent PID: %d\n", (int)getpid());

        wait(NULL);

        printf("Child finished\n");

        sleep(30);
    }

    return 0;
}
