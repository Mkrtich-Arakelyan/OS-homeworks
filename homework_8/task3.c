#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char path[4096];
    char byte;
    int result = 1;

    printf("File path: ");
    fflush(stdout);
    if (fgets(path, sizeof path, stdin) == NULL) {
        fprintf(stderr, "Could not read the file path.\n");
        return 1;
    }
    path[strcspn(path, "\n")] = '\0';

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    off_t pos = lseek(fd, 0, SEEK_END);
    if (pos == -1) {
        perror("lseek to end");
        goto cleanup;
    }

    while (pos > 0) {
        --pos;

        if (lseek(fd, pos, SEEK_SET) == -1) {
            perror("lseek");
            goto cleanup;
        }

        ssize_t n = read(fd, &byte, 1);
        if (n == -1) {
            perror("read");
            goto cleanup;
        }
        if (n == 0) {
            fprintf(stderr, "Unexpected end of file.\n");
            goto cleanup;
        }

        if (write(STDOUT_FILENO, &byte, 1) != 1) {
            perror("write");
            goto cleanup;
        }
    }

    if (write(STDOUT_FILENO, "\n", 1) != 1) {
        perror("write newline");
        goto cleanup;
    }
    result = 0;

cleanup:
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return result;
}
