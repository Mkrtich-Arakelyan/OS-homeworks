#include <stdio.h>
#include <stddef.h>

struct Normal {
    char a;
    int b;
    double c;
};


#pragma pack(push, 1)
struct Packed {
    char a;
    int b;
    double c;
};
#pragma pack(pop) 

int main(void) {
    struct Normal normal = {'A', 10, 3.14};
    struct Packed packed = {'A', 10, 3.14};

    printf("Normal struct\n");
    printf("Size: %zu, alignment: %zu\n",
           sizeof(normal), _Alignof(struct Normal));

    printf("a: address = %p, offset = %zu\n",
           (void *)&normal.a, offsetof(struct Normal, a));
    printf("b: address = %p, offset = %zu\n",
           (void *)&normal.b, offsetof(struct Normal, b));
    printf("c: address = %p, offset = %zu\n",
           (void *)&normal.c, offsetof(struct Normal, c));

    printf("\nPacked struct\n");
    printf("Size: %zu, alignment: %zu\n",
           sizeof(packed), _Alignof(struct Packed));

    printf("a: address = %p, offset = %zu\n",
           (void *)&packed.a, offsetof(struct Packed, a));
    printf("b: address = %p, offset = %zu\n",
           (void *)&packed.b, offsetof(struct Packed, b));
    printf("c: address = %p, offset = %zu\n",
           (void *)&packed.c, offsetof(struct Packed, c));

    return 0;
}
