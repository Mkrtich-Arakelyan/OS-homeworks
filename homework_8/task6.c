#include <errno.h>
#include <fcntl.h>
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
    const char original[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n";
    char remainder[sizeof original];
    char buffer[64];
    int result = 1;

    int fd = open("numbers.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open for writing");
        return 1;
    }

    if (write_all(fd, original, sizeof original - 1) == -1)
        goto cleanup;

    if (close(fd) == -1) {
        perror("close after writing");
        return 1;
    }

    fd = open("numbers.txt", O_RDWR);
    if (fd == -1) {
        perror("reopen");
        return 1;
    }

    off_t offset = 0;
    off_t line4_start = 0;
    int newlines = 0;

    /* Scan through line 4, remembering where it starts. */
    while (newlines < 4) {
        char ch;
        ssize_t n = read(fd, &ch, 1);
        if (n == -1) {
            perror("read while scanning");
            goto cleanup;
        }
        if (n == 0) {
            fprintf(stderr, "Unexpected end of file\n");
            goto cleanup;
        }

        ++offset;
        if (ch == '\n' && ++newlines == 3)
            line4_start = offset;
    }

    /* The file position is now at the start of line 5. */
    size_t saved = 0;
    ssize_t n;
    while ((n = read(fd, buffer, sizeof buffer)) > 0) {
        if ((size_t)n > sizeof remainder - saved) {
            fprintf(stderr, "Remainder is too large\n");
            goto cleanup;
        }
        for (ssize_t i = 0; i < n; ++i)
            remainder[saved++] = buffer[i];
    }
    if (n == -1) {
        perror("read remainder");
        goto cleanup;
    }

    if (lseek(fd, line4_start, SEEK_SET) == -1) {
        perror("lseek to line 4");
        goto cleanup;
    }

    if (write_all(fd, "100\n", 4) == -1 ||
        write_all(fd, remainder, saved) == -1)
        goto cleanup;

    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("rewind");
        goto cleanup;
    }

    while ((n = read(fd, buffer, sizeof buffer)) > 0) {
        if (write_all(STDOUT_FILENO, buffer, (size_t)n) == -1)
            goto cleanup;
    }
    if (n == -1) {
        perror("read final content");
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
