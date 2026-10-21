#include <stdio.h>

int main(void) {
    int number = 10;
    char letter = 'A';
    double decimal = 3.14;
    short small = 5;

    printf("int:    size = %zu, alignment = %zu, address = %p\n",
           sizeof(number), _Alignof(int), (void *)&number);

    printf("char:   size = %zu, alignment = %zu, address = %p\n",
           sizeof(letter), _Alignof(char), (void *)&letter);

    printf("double: size = %zu, alignment = %zu, address = %p\n",
           sizeof(decimal), _Alignof(double), (void *)&decimal);

    printf("short:  size = %zu, alignment = %zu, address = %p\n",
           sizeof(small), _Alignof(short), (void *)&small);

    return 0;
}

