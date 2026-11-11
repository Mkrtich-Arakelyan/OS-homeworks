#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    const char text[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buffer[32];
    int result = 1;

    int fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open for writing");
        return 1;
    }

    size_t written = 0;
    while (written < sizeof text - 1) {
        ssize_t n = write(fd, text + written, sizeof text - 1 - written);
        if (n == -1) {
            perror("write");
            goto cleanup;
        }
        if (n == 0) {
            fprintf(stderr, "write made no progress\n");
            goto cleanup;
        }
        written += (size_t)n;
    }

    if (close(fd) == -1) {
        perror("close after writing");
        return 1;
    }

    fd = open("data.txt", O_RDWR);
    if (fd == -1) {
        perror("reopen");
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("lseek before truncation");
        goto cleanup;
    }
    printf("Original size: %jd bytes\n", (intmax_t)size);

    if (ftruncate(fd, 10) == -1) {
        perror("ftruncate");
        goto cleanup;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("lseek after truncation");
        goto cleanup;
    }
    printf("New size: %jd bytes\n", (intmax_t)size);

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("rewind");
        goto cleanup;
    }

    printf("Remaining content: ");
    ssize_t n;
    while ((n = read(fd, buffer, sizeof buffer)) > 0) {
        printf("%.*s", (int)n, buffer);
    }
    if (n == -1) {
        perror("read");
        goto cleanup;
    }
    putchar('\n');
    result = 0;

cleanup:
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return result;
}
