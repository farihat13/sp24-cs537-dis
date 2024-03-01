/**
 * This example 
 * a) creates an array of N integers in the parent using mmap with MAP_PRIVATE flag,
 *    0, 10, 20, ....
 * b) and changes the first element in the child process.
 *    99, 10, 20, ...
 * But, the parent can NOT see the change because of the MAP_PRIVATE flag.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>


void print_arr(int *arr, int N){
    // Print the elements of the array
    printf("Array ");
    for (int i = 0; i < N; i++)
        printf("[%d] ", arr[i]);
    printf("\n");
}

int main() {

    int N = 5;

    // malloc an array of N integers using mmap
    void *addr = NULL;
    size_t len = N * sizeof(int);
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    int fd = -1;
    off_t offset = 0;

    int *arr = (int *)mmap(addr, len, prot, flags, fd, offset);
    if (arr == MAP_FAILED) {
        printf("Mapping Failed\n");
        return 1;
    }

    // Fill the elements of the array
    for (int i = 0; i < N; i++)
        arr[i] = i * 10;

    int pid = fork();
    if (pid == 0) {
        printf("Inside child\n");
        // print the array
        print_arr(arr, N);

        // change the first element
        arr[0] = 99;

        // print the array
        print_arr(arr, N);
    } else {
        waitpid(pid, NULL, 0);

        printf("Inside parent\n");
        // print the array
        print_arr(arr, N);
    }

    // free the memory
    int ret = munmap(arr, len);
    if (ret != 0) {
        printf("UnMapping Failed\n");
        return 1;
    }

    return 0;
}
