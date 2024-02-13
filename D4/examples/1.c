/**
 * 1. the fork returns the pid of the child process to the parent process
 * 2. the ppid of the child process is the pid of the parent process
 * 3. the parent and child process have the same pgid, which is the pid of the parent process
 *    which means the parent and child process are in the same process group
 * `echo $$` or `echo $BASHPID` in shell will print the pid of the shell
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void child(int n) {
    printf("child%d pid: %d, ppid: %d, pgid: %d\n", n, getpid(), getppid(), getpgrp());
}

int main() {
    printf("Hello, World!\n");

    int pid1 = fork();
    if (pid1 == 0) {
        child(1);
    } else {
        int pid2 = fork();
        if (pid2 == 0) {
            child(2);
        } else {
            wait(NULL);
            printf("fork returns: pid1: %d, pid2: %d\n", pid1, pid2);
            printf("parent pid: %d, ppid: %d, pgid: %d\n", getpid(), getppid(), getpgrp());
        }
    }
    return 0;
}