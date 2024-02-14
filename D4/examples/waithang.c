// https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-waitpid-wait-specific-child-process-endßß


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/wait.h>

// WNOHANG
// Demands status information immediately. 
// If status information is immediately available on an appropriate child process, 
// waitpid() returns this information. 
// Otherwise, waitpid() returns immediately with an error code indicating that 
// the information was not available. In other words, WNOHANG checks child processes 
// without causing the caller to be suspended.

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
        waitpid(pid, &status, WNOHANG);
        printf("I'm the Parent. Child's status %d\n", status);

        waitpid(pid, &status, 0);
    }
    return 0;
}