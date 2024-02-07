#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
	int pid = fork();
    if (pid==0) { // this is true the child process
        printf("I am the child process with pid %d. ", getpid());
        printf("I got the return value %d from fork.\n", pid);
    } else { // this is true for the parent process
        int status;
        waitpid(pid, &status, 0);
        printf("I am the parent process with pid %d. ", getpid());
        printf("I got the return value %d from fork.\n", pid);
    }
	return 0;
}
