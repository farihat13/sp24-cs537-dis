// https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-waitpid-wait-specific-child-process-endßß


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/wait.h>

// Status analysis macros: 
// If the status_ptr argument is not NULL, 
// waitpid() places the child's return status in *status_ptr. 
// You can analyze this return status with the following macros,
//  defined in the sys/wait.h header file:

int main()
{
    pid_t pid = fork();
    if (pid == 0) {
        char *const argv[] = {"/bin/sleep", "5", NULL};  
        int ret = execv(argv[0], argv);
        printf("Fails to execute %s\n", argv[0]);
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFCONTINUED(status))
            printf("WIFCONTINUED(status) is true\n");
        else if (WIFEXITED(status))
            printf("WIFEXITED(status) is true\n");
        else if (WIFSIGNALED(status))
            printf("WIFSIGNALED(status) is true\n");
        else if (WIFSTOPPED(status))
            printf("WIFSTOPPED(status) is true\n");
        else
            printf("None of the above status is true\n");
    }
    return 0;
}