/**
 * https://jameshfisher.com/2017/01/28/mmap-file-write/
 * This example 
 * a) maps a file using mmap,
 * b) and reads the file through the mmapped memory.
 */

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * Assumption: file size > testtowrite_size
 * S_IRUSR  00400 user has read permission
 * S_IWUSR  00200 user has write permission
 */
int main(void) {
    
    const char *filename = "hello.txt";

    // open file
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("failed to open %s\n", filename);
        return 1;
    }

    printf("--- file read normally ---\n");
    int fsize = 0, b;
    char line[256];
    while ((b = read(fd, line, sizeof(line))) > 0) {
        printf("%s", line);
        fsize += b;
    }
    printf("--- done ---\n");


    // mmap this file 
    void *addr = NULL;
    size_t len = fsize;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE;
    off_t offset = 0;
    char *map = mmap(addr, fsize, prot, flags, fd, offset);

    // close the file descriptor
    close(fd);

    // read the file using mmap
    printf("--- file read through mmap ---\n");
    int i;
    for (i = 0; i < fsize; i++)
        printf("%c", map[i]);
    printf("\n--- done ---\n");

    // unmap
    munmap(map, fsize);
    return 0;
}