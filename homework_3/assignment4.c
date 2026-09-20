#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child1, child2;
    int status1, status2;

    child1 = fork();

    if (child1 < 0) {
        perror("fork failed");
        return 1;
    }

    if (child1 == 0) {
        printf("Child 1 PID: %d\n", (int)getpid());
        exit(0);
    }

    child2 = fork();

    if (child2 < 0) {
        perror("fork failed");
        return 1;
    }

    if (child2 == 0) {
        printf("Child 2 PID: %d\n", (int)getpid());
        exit(1);
    }

    waitpid(child1, &status1, 0);
    waitpid(child2, &status2, 0);

    printf("Child 1 status: %d\n", status1);
    printf("Child 2 status: %d\n", status2);

    if (status1 == 0)
        printf("Child 1 exited successfully\n");
    else
        printf("Child 1 exited with an error\n");

    if (status2 == 0)
        printf("Child 2 exited successfully\n");
    else
        printf("Child 2 exited with an error\n");

    return 0;
}
