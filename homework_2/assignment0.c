#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t f1, f2, f3;

    f1 = fork();

    if (f1 == -1) {
        perror("fork 1 failed");
        return 1;
    }

    f2 = fork();

    if (f2 == -1) {
        perror("fork 2 failed");
        return 1;
    }

    f3 = fork();

    if (f3 == -1) {
        perror("fork 3 failed");
        return 1;
    }

    printf("PID: %d, PPID: %d, f1: %d, f2: %d, f3: %d\n",
           (int)getpid(),
           (int)getppid(),
           (int)f1,
           (int)f2,
           (int)f3);

    return 0;
}
