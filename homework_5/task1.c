#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    long long sum = 0;

    printf("Enter the number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Please enter a positive number of elements.\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid integer input.\n");
            free(arr);
            return 1;
        }
        sum += arr[i];
    }

    printf("Sum of the array: %lld\n", sum);
    free(arr);

    return 0;
}
