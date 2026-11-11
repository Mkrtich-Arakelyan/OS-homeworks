#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char source[4096], destination[4096];
    char buffer[4096];
    size_t total = 0;
    int failed = 0;

    printf("Source file path: ");
    fflush(stdout);
    if (fgets(source, sizeof source, stdin) == NULL) {
        fprintf(stderr, "Could not read the source path.\n");
        return 1;
    }
    source[strcspn(source, "\n")] = '\0';

    printf("Destination file path: ");
    fflush(stdout);
    if (fgets(destination, sizeof destination, stdin) == NULL) {
        fprintf(stderr, "Could not read the destination path.\n");
        return 1;
    }
    destination[strcspn(destination, "\n")] = '\0';

    int src = open(source, O_RDONLY);
    if (src == -1) {
        perror("open source");
        return 1;
    }

    int dst = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst == -1) {
        perror("open destination");
        if (close(src) == -1)
            perror("close source");
        return 1;
    }

    for (;;) {
        ssize_t n = read(src, buffer, sizeof buffer);

        if (n == 0)
            break;  // End of file

        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("read");
            failed = 1;
            break;
        }

        ssize_t written = 0;
        while (written < n) {
            ssize_t w = write(dst, buffer + written, (size_t)(n - written));

            if (w == -1 && errno == EINTR)
                continue;
            if (w <= 0) {
                if (w == -1)
                    perror("write");
                else
                    fprintf(stderr, "write made no progress.\n");
                failed = 1;
                break;
            }

            written += w;
            total += (size_t)w;
        }

        if (failed)
            break;
    }

    if (close(src) == -1) {
        perror("close source");
        failed = 1;
    }
    if (close(dst) == -1) {
        perror("close destination");
        failed = 1;
    }

    if (failed)
        return 1;

    printf("Total bytes copied: %zu\n", total);
    return 0;
}
