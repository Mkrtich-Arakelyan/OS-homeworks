#include <stdio.h>
#include <stdlib.h>

void first_function(void)
{
    printf("First cleanup function\n");
}

void second_function(void)
{
    printf("Second cleanup function\n");
}

int main(void)
{
    if (atexit(first_function) != 0) {
        printf("atexit failed\n");
        return 1;
    }

    if (atexit(second_function) != 0) {
        printf("atexit failed\n");
        return 1;
    }

    printf("Program is running\n");

    exit(0);
}
