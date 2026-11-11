#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    char entry[4096];

    int prefix = snprintf(entry, sizeof entry, "PID=%jd: ",
                          (intmax_t)getpid());
    if (prefix < 0 || (size_t)prefix >= sizeof entry - 1) {
        fprintf(stderr, "Could not format the PID prefix.\n");
        return 1;
    }
    size_t length = (size_t)prefix;

    printf("Enter a log line: ");
    fflush(stdout);

    for (;;) {
        char ch;
        ssize_t n = read(0, &ch, 1);

        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("read");
            return 1;
        }

        if (n == 0 || ch == '\n')
            break;

        if (length == sizeof entry - 1) {
            fprintf(stderr, "Log line is too long.\n");
            return 1;
        }
        entry[length++] = ch;
    }

    entry[length++] = '\n';

    int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open log.txt");
        return 1;
    }

    int result = 1;
    size_t written = 0;
    while (written < length) {
        ssize_t n = write(fd, entry + written, length - written);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("write");
            goto cleanup;
        }
        if (n == 0) {
            fprintf(stderr, "write made no progress.\n");
            goto cleanup;
        }
        written += (size_t)n;
    }

    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset == -1) {
        perror("lseek");
        goto cleanup;
    }

    printf("Final file offset: %jd bytes\n", (intmax_t)offset);
    result = 0;

cleanup:
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return result;
}
