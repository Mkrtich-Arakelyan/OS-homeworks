#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

static int write_all(int fd, const char *data, size_t length) {
    while (length > 0) {
        ssize_t n = write(fd, data, length);

        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("write");
            return -1;
        }
        if (n == 0) {
            fprintf(stderr, "write made no progress\n");
            return -1;
        }

        data += n;
        length -= (size_t)n;
    }
    return 0;
}

int main(void) {
    int fd = open("sparse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open for writing");
        return 1;
    }

    int result = 1;

    if (write_all(fd, "START", 5) == -1)
        goto cleanup;

    /*
     * Seeking skips bytes without writing zeros. Writing END then
     * extends the file and leaves a hole. The hole counts toward
     * apparent size, but normally needs no disk blocks, so du
     * reports much less space used.
     */
    if (lseek(fd, 1024 * 1024, SEEK_CUR) == -1) {
        perror("lseek forward");
        goto cleanup;
    }

    if (write_all(fd, "END", 3) == -1)
        goto cleanup;

    if (close(fd) == -1) {
        perror("close after writing");
        return 1;
    }

    fd = open("sparse.bin", O_RDONLY);
    if (fd == -1) {
        perror("reopen");
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("lseek to end");
        goto cleanup;
    }

    printf("Apparent file size: %jd bytes\n", (intmax_t)size);
    result = 0;

cleanup:
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return result;
}
