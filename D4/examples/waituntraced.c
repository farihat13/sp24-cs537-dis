// https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-waitpid-wait-specific-child-process-endßß


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/wait.h>

// WUNTRACED
// Reports on stopped child processes as well as terminated ones. 
// The WIFSTOPPED macro lets a process distinguish between 
// a stopped process and a terminated one.

int main()
{
    pid_t pid = fork();
    if (pid == 0) {
        char *const argv[3] = {"/bin/sleep", "5", NULL};  
        int ret = execv(argv[0], argv);
        printf("Fails to execute %s\n", argv[0]);
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, WUNTRACED);
        printf("I'm the Parent. Child's status %d\n", status);
    }
    return 0;
}