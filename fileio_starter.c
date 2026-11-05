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

/* Number of lines per file. Default 25; override at compile time,
 * e.g.  gcc -DN=1000 ...  */
#ifndef N
#define N 25
#endif

void write_raw(const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    const char *line = "raw line 0\n";
    write(fd, line, strlen(line));

    /* Part A: lines 1 .. N-1, one separate write() syscall per line.
     * write() only takes raw bytes, so format each line with snprintf first. */
    char buf[64];
    for (int i = 1; i < N; i++) {
        int len = snprintf(buf, sizeof buf, "raw line %d\n", i);
        write(fd, buf, len);
    }

    close(fd);
}

void write_buffered(const char *path) {
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        perror("fopen");
        return;
    }

    fprintf(fp, "buffered line 0\n");

    /* Part A: lines 1 .. N-1, one fprintf() call per line.
     * These go into stdio's user-space buffer, not straight to the kernel. */
    for (int i = 1; i < N; i++) {
        fprintf(fp, "buffered line %d\n", i);
    }

    fclose(fp);
}

/* Usage:  ./fileio            -> both (Part A)
 *         ./fileio raw        -> only write_raw      (Part B, separate runs)
 *         ./fileio buffered   -> only write_buffered (Part B, separate runs) */
int main(int argc, char *argv[]) {
    int do_raw = 1, do_buf = 1;
    if (argc > 1) {
        do_raw = (strcmp(argv[1], "raw") == 0);
        do_buf = (strcmp(argv[1], "buffered") == 0);
    }

    if (do_raw) write_raw("raw_output.txt");
    if (do_buf) write_buffered("buffered_output.txt");
    printf("done -- check raw_output.txt and buffered_output.txt\n");
    return 0;
}