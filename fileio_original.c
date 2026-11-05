#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * Starter skeleton for Workshop 8 (File I/O: Raw Syscalls vs. Buffered stdio).
 *
 * As given, this writes ONE line to a file using raw syscalls, and ONE line
 * to a (different) file using buffered stdio -- just enough to confirm both
 * paths work. Extending each to write MANY lines, and observing/comparing
 * their behavior, is your job for Part A/B.
 *
 * TODOs mark exactly where your work starts.
 */

void write_raw(const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    const char *line = "raw line 0\n";
    write(fd, line, strlen(line));

    /* TODO (Part A): write N lines instead of just one -- wrap this in a
     * loop, writing "raw line 1\n", "raw line 2\n", ... "raw line N-1\n"
     * (one separate write() call per line). */

    close(fd);
}

void write_buffered(const char *path) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror("fopen");
        return;
    }

    fprintf(fp, "buffered line 0\n");

    /* TODO (Part A): write N lines instead of just one, the same way
     * write_raw() does -- fprintf() in a loop, one call per line. */

    fclose(fp);
}

int main(void) {
    write_raw("raw_output.txt");
    write_buffered("buffered_output.txt");
    printf("done -- check raw_output.txt and buffered_output.txt\n");
    return 0;
}
