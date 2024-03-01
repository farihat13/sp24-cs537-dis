/**
 * This example
 * a) mmaps a file in MAP_SHARED mode in the parent,
 * b) writes to the mmapped memory in the child,
 * c) reads the mmapped memory in the parent,
 * d) and reads the file in the parent.
 * The write by child is reflected in the parent because of MAP_SHARED.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int readfile(int fd) {
    int ret = lseek(fd, 0, SEEK_SET);
    if (ret < 0) {
        printf("fseek failed\n");
        exit(1);
    }
    printf("--- file read ---\n");
    int fsize = 0, b;
    char line[256];
    while ((b = read(fd, line, sizeof(line))) > 0) {
        printf("%s", line);
        fsize += b;
    }
    printf("\n--- done ---\n");
    return fsize;
}

/**
 * Assumption: file size > testtowrite_size
 */
int main(void) {
    const char *filename = "hello.txt";

    // open file
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("failed to open %s\n", filename);
        return 1;
    }

    // get filesize
    struct stat s;
    int ret = fstat(fd, &s);
    if (ret < 0) {
        printf("fstat failed\n");
        return 1;
    }
    int fsize = s.st_size;

    // mmap this file
    void *addr = NULL;
    size_t len = fsize;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED;
    off_t offset = 0;
    char *map = mmap(addr, fsize, prot, flags, fd, offset);

    int pid = fork();
    if (pid == 0) {
        printf("Inside child\n");
        
        // read file inside child
        readfile(fd);

        // this write will NOT be reflected since the flag is MAP_PRIVATE
        printf("--- write to mmapped memory ---\n");
        const char *texttowrite = "wrote this line via mmap in child";
        memcpy(map, texttowrite, strlen(texttowrite));

        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        printf("Inside parent\n");

        // read the file using mmap
        printf("--- file read through mmap ---\n");
        int i;
        for (i = 0; i < fsize; i++)
            printf("%c", map[i]);
        printf("\n--- done ---\n");

        // read file inside parent
        readfile(fd);

        // unmap
        munmap(map, fsize);
        // close the file
        close(fd);

        return 0;
    }
}