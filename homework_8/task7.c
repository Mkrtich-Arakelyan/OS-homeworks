#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int get_path(const char *prompt, char *path, size_t capacity) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(path, capacity, stdin) == NULL) {
        fprintf(stderr, "Could not read the file path.\n");
        return -1;
    }

    path[strcspn(path, "\n")] = '\0';
    return 0;
}

/* Fill the buffer unless we reach EOF or encounter an error. */
static ssize_t read_chunk(int fd, unsigned char *buffer, size_t capacity) {
    size_t total = 0;

    while (total < capacity) {
        ssize_t n = read(fd, buffer + total, capacity - total);

        if (n == 0)
            break;
        if (n == -1) {
            if (errno == EINTR)
                continue;
            return -1;
        }

        total += (size_t)n;
    }

    return (ssize_t)total;
}

int main(void) {
    char path1[4096], path2[4096];
    unsigned char buffer1[4096], buffer2[4096];

    if (get_path("First file path: ", path1, sizeof path1) == -1 ||
        get_path("Second file path: ", path2, sizeof path2) == -1)
        return 2;

    int fd1 = open(path1, O_RDONLY);
    if (fd1 == -1) {
        perror("open first file");
        return 2;
    }

    int fd2 = open(path2, O_RDONLY);
    if (fd2 == -1) {
        perror("open second file");
        if (close(fd1) == -1)
            perror("close first file");
        return 2;
    }

    uintmax_t offset = 0;
    int result = 2;

    for (;;) {
        ssize_t n1 = read_chunk(fd1, buffer1, sizeof buffer1);
        if (n1 == -1) {
            perror("read first file");
            break;
        }

        ssize_t n2 = read_chunk(fd2, buffer2, sizeof buffer2);
        if (n2 == -1) {
            perror("read second file");
            break;
        }

        size_t common = (size_t)(n1 < n2 ? n1 : n2);
        size_t i = 0;

        while (i < common && buffer1[i] == buffer2[i])
            ++i;

        if (i < common || n1 != n2) {
            printf("Files differ at byte %ju\n", offset + i);
            result = 1;
            break;
        }

        if (n1 == 0) {
            puts("Files are identical");
            result = 0;
            break;
        }

        offset += common;
    }

    if (close(fd1) == -1) {
        perror("close first file");
        result = 2;
    }
    if (close(fd2) == -1) {
        perror("close second file");
        result = 2;
    }

    return result;
}
