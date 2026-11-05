#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * Part B, Task 3: the fflush() fix.
 *
 * Same crash test as before, but fflush(fp) is called right after the
 * buffered writes, BEFORE the sleep. Kill it with kill -9 during the sleep
 * and compare the two files.
 */

#define LINES 10

int main(void) {
    /* Raw: open the file, one write() syscall per line */
    int fd = open("raw_crash.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("open"); return 1; }

    /* Buffered: fopen the file, one fprintf() per line */
    FILE *fp = fopen("buffered_crash.txt", "w");
    if (fp == NULL) { perror("fopen"); return 1; }

    char buf[64];
    for (int i = 0; i < LINES; i++) {
        int len = snprintf(buf, sizeof buf, "raw line %d\n", i);
        write(fd, buf, len);                     /* goes straight to the kernel */
    }
    for (int i = 0; i < LINES; i++) {
        fprintf(fp, "buffered line %d\n", i);    /* stays in stdio's buffer... */
    }

    fflush(fp);   /* THE FIX: push stdio's buffer into the kernel NOW */

    /* still NO fclose(fp), NO close(fd) yet */
    printf("PID %d: wrote %d lines to each file, sleeping 3s -- kill -9 me now!\n",
           getpid(), LINES);
    fflush(stdout);   /* only flushes the terminal message */

    sleep(3);

    /* Only reached if the process was NOT killed */
    fclose(fp);
    close(fd);
    printf("finished normally (not killed)\n");
    return 0;
}