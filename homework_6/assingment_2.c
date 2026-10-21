#include <stdio.h>
#include <stddef.h>

struct Original {
    char a;
    int b;
    double c;
};

struct Reordered {
    double c;
    int b;
    char a;
};

int main(void) {
    struct Original first = {'A', 10, 3.14};
    struct Reordered second = {3.14, 10, 'A'};

    printf("Original: char, int, double\n");
    printf("Size: %zu bytes\n", sizeof(first));

    printf("a: address = %p, offset = %zu\n",
           (void *)&first.a, offsetof(struct Original, a));
    printf("b: address = %p, offset = %zu\n",
           (void *)&first.b, offsetof(struct Original, b));
    printf("c: address = %p, offset = %zu\n",
           (void *)&first.c, offsetof(struct Original, c));

    printf("\nReordered: double, int, char\n");
    printf("Size: %zu bytes\n", sizeof(second));

    printf("c: address = %p, offset = %zu\n",
           (void *)&second.c, offsetof(struct Reordered, c));
    printf("b: address = %p, offset = %zu\n",
           (void *)&second.b, offsetof(struct Reordered, b));
    printf("a: address = %p, offset = %zu\n",
           (void *)&second.a, offsetof(struct Reordered, a));

    return 0;
}
