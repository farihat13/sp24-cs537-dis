/**
 * This example creates an array of N integers using mmap.
 * The mmap can be similar to malloc when used with the
 * MAP_ANONYMOUS and MAP_PRIVATE flags.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PGSIZE 4096

int print_array_sum(char *arr, int N) {
    int sum = 0;
    for (int i = 0; i < N; i++)
        sum += arr[i];
    return sum;
}

int main() {
    void *addr = NULL;
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    int fd = -1;
    off_t offset = 0;

    // malloc an array of n1 integers using mmap
    size_t len = 2 * PGSIZE;
    char *arr1 = (char *)mmap(addr, len, prot, flags, fd, offset);
    if (arr1 == MAP_FAILED) {
        printf("Mapping Failed\n");
        return 1;
    }
    printf("Map 1 at address %p\n", arr1);

    char *arr2 = (char *)mmap(addr, len, prot, flags, fd, offset);
    if (arr2 == MAP_FAILED) {
        printf("Mapping Failed\n");
        return 1;
    }
    printf("Map 2 at address %p\n", arr2);

    // write to the first array
    for (int i = 0; i < len; i++)
        arr1[i] = 'a';
    printf("Map 1 Sum: %d\n", print_array_sum(arr1, len));
    // write to the second array
    for (int i = 0; i < len; i++)
        arr2[i] = 'b';
    printf("Map 2 Sum: %d\n", print_array_sum(arr2, len));

    // remap the second array to a larger size
    int remap_flags = MREMAP_MAYMOVE; // TODO: change this to 0
    int newlen = 4 * PGSIZE;
    char *arr3 = (char *)mremap(arr2, 2 * PGSIZE, newlen, remap_flags);
    if (arr3 == MAP_FAILED) {
        printf("Remapping Failed\n");
        return 1;
    }
    printf("Remapped Map 2 at address %p\n", arr3);
    printf("Map 2 Sum:  %d\n", print_array_sum(arr3, len));

    // unmap both
    if (munmap(arr1, len) == -1) {
        printf("Unmapping Failed\n");
        return 1;
    }
    if (munmap(arr3, newlen) == -1) {
        printf("Unmapping Failed\n");
        return 1;
    }

    return 0;
}