#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int initial_size = 10;
    int new_size = 5;

    int *arr = malloc(initial_size * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter %d integers: ", initial_size);
    for (int i = 0; i < initial_size; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid integer input.\n");
            free(arr);
            return 1;
        }
    }

    int *new_arr = realloc(arr, new_size * sizeof(int));
    if (new_arr == NULL) {
        printf("Memory reallocation failed!\n");
        free(arr);
        return 1;
    }
    arr = new_arr;

    printf("Array after resizing:");
    for (int i = 0; i < new_size; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
