/**
 * https://jameshfisher.com/2017/01/28/mmap-file-write/
 * This example 
 * a) mmaps a file in MAP_PRIVATE mode,
 * b) and writes to the mmapped file.
 * But, the write is NOT reflected in the original file.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>

int readfile(int fd) {
    int ret = lseek(fd, 0, SEEK_SET);
    if (ret < 0) {
        printf("fseek failed\n");
        return -1;
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
    int flags = MAP_PRIVATE;
    off_t offset = 0;
    char *map = mmap(addr, fsize, prot, flags, fd, offset);

    // read file normally from the beginning
    readfile(fd);

    // write
    // this write will NOT be reflected since the flag is MAP_PRIVATE
    printf("--- file write through mmap ---\n");
    const char *texttowrite = "wrote this line via mmap";
    size_t textsize = strlen(texttowrite);
    memcpy(map, texttowrite, strlen(texttowrite));

    // unmap
    munmap(map, fsize);

    // read file normally from the beginning
    readfile(fd);
    close(fd);

    return 0;
}