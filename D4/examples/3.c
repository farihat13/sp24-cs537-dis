/**
 * If we wait for child2 to finish, child1 might not have finished
 * by the time the parent process finishes.
 * https://linux.die.net/man/2/waitpid
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

char *const cmd1[3] = {"sleep", "10", NULL};
char *const cmd2[3] = {"sleep", "2", NULL};
char *const cmd[3] = {"ls", "-l", NULL};

void child(int n) {
    printf("child%d pid: %d, ppid: %d, pgid: %d\n", n, getpid(), getppid(), getpgrp());
    if (n == 1) {
        int ret = execvp(cmd1[0], cmd1);
        printf("Fails to execute %s by child%d\n", cmd1[0], n);
    } else {
        int ret = execvp(cmd2[0], cmd2);
        printf("Fails to execute %s by child%d\n", cmd2[0], n);
    }
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
            printf("fork returns: pid1: %d, pid2: %d\n", pid1, pid2);
            int status, wpid;

            while ((wpid = waitpid(-1, &status, 0)) > 0) {
                printf("Child %d exited with status %d\n", wpid, WEXITSTATUS(status));
            }
            // printf("Child %d exited with status %d\n", wpid, WEXITSTATUS(status));

            printf("parent pid: %d, ppid: %d, pgid: %d\n", getpid(), getppid(), getpgrp());
        }
    }
    return 0;
}