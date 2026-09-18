#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child1, child2;

    // First child
    child1 = fork();

    if (child1 < 0) {
        perror("fork failed");
        return 1;
    }

    if (child1 == 0) {
        execl("/bin/ls", "ls", NULL);

        perror("execl failed");
        return 1;
    }

    // Parent waits until ls finishes
    waitpid(child1, NULL, 0);

    // Second child
    child2 = fork();

    if (child2 < 0) {
        perror("fork failed");
        return 1;
    }

    if (child2 == 0) {
        execl("/bin/date", "date", NULL);

        perror("execl failed");
        return 1;
    }

    // Parent waits until date finishes
    waitpid(child2, NULL, 0);

    printf("Parent process done\n");

    return 0;
}
