#include <stdio.h>
#include <stdlib.h>

void free_strings(char **strings, int count) {
    for (int i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);
}

int main(void) {
    int initial_size = 3;
    int new_size = 5;
    int string_size = 51;

    char **strings = malloc(initial_size * sizeof(char *));
    if (strings == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter %d strings: ", initial_size);
    for (int i = 0; i < initial_size; i++) {
        strings[i] = malloc(string_size * sizeof(char));
        if (strings[i] == NULL) {
            printf("Memory allocation failed!\n");
            free_strings(strings, i);
            return 1;
        }

        if (scanf("%50s", strings[i]) != 1) {
            printf("Invalid string input.\n");
            free_strings(strings, i + 1);
            return 1;
        }
    }

    printf("Initial strings:");
    for (int i = 0; i < initial_size; i++) {
        printf(" %s", strings[i]);
    }
    printf("\n");

    char **new_strings = realloc(strings, new_size * sizeof(char *));
    if (new_strings == NULL) {
        printf("Memory reallocation failed!\n");
        free_strings(strings, initial_size);
        return 1;
    }
    strings = new_strings;

    printf("Enter %d more strings: ", new_size - initial_size);
    for (int i = initial_size; i < new_size; i++) {
        strings[i] = malloc(string_size * sizeof(char));
        if (strings[i] == NULL) {
            printf("Memory allocation failed!\n");
            free_strings(strings, i);
            return 1;
        }

        if (scanf("%50s", strings[i]) != 1) {
            printf("Invalid string input.\n");
            free_strings(strings, i + 1);
            return 1;
        }
    }

    printf("All strings:");
    for (int i = 0; i < new_size; i++) {
        printf(" %s", strings[i]);
    }
    printf("\n");

    free_strings(strings, new_size);
    return 0;
}
