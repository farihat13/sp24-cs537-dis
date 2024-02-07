/**
 * Executes the command "cat scores | grep bob". 
 */
#include <stdio.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/wait.h>
int main(int argc, char *argv[])
{
    // make a pipe
    // pipefd[0] will be the fd(file descriptor) for the read end of pipe.
    // pipefd[1] will be the fd for the write end of pipe.
    // returns 0 on Success.
    int pipefd[2];
    pipe(pipefd);
    int pid = fork();
    if (pid == 0) // child handles "grep bob"
    {
        // replace standard input with input part of pipe 
        dup2(pipefd[0], 0);
        close(pipefd[1]);
        // execute grep
        char *grep_args[] = {"/bin/grep", "bob", NULL}; // grep bob
        execv(grep_args[0], grep_args);
    } else { // parent handles "cat scores"
        // replace standard output with output part of pipe 
        dup2(pipefd[1], 1);
        // close unused innput half of pipe
        close(pipefd[0]);
        // execute cat
        char *cat_args[] = {"/bin/cat", "scores.txt", NULL}; // cat scores
        execv(cat_args[0], cat_args);
        perror(cat_args[0]);
    }
    return 0;
}