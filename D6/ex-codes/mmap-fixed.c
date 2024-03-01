/**
 * This example creates an array of N integers at a fixed address 
 * using mmap with MAP_FIXED.
 */

#include <sys/types.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // void *goal_addr = (void *)0x10000;
    // size_t length = 1 << 10;

    int N = 5;

    // malloc an array of N integers using mmap
    void *addr = (void *)0x10000;
    size_t len = N * sizeof(int);
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED;
    int fd = -1;
    off_t offset = 0;

    int *arr = (int *)mmap(addr, len, prot, flags, fd, offset);
    if (arr == MAP_FAILED) {
        printf("Mapping Failed\n");
        return 1;
    }

    if (arr != addr) {
        // this will not happen.
        printf("Failed to map at given address\n");
    }

    printf("Mapped address %p\n", addr);

    // Fill the elements of the array
    for (int i = 0; i < N; i++)
        arr[i] = i * 10;

    // Print the elements of the array
    printf("Array ");
    for (int i = 0; i < N; i++)
        printf("[%d] ", arr[i]);
    printf("\n");

    // free the memory
    int ret = munmap(arr, len);
    if (ret != 0) {
        printf("UnMapping Failed\n");
        return 1;
    }

    return 0;
}