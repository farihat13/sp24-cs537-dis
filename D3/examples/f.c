
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 
#include <sys/wait.h>
int main()
{
    pid_t pid = fork();
    if (pid == 0) {
        pid = fork();
        if (pid == 0) {
            printf("exec cd ..\n");
            char *const argv[3] = {"/bin/echo", // string literial is of type "const char*"
                                "hello",
                                NULL // it must have a NULL in the end of argv
                            };  
            int ret = execv(argv[0], argv);
            exit(1);
        } else {
            waitpid(pid, NULL, 0);
            printf("exec ls -l\n");
            char *const argv[3] = {"/bin/ls", // string literial is of type "const char*"
                                "-l",
                                NULL // it must have a NULL in the end of argv
                            };  
            int ret = execv(argv[0], argv);
            exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        printf("I'm the Parent\n");
    }
    return 0;
}
// file: points to the file name associated with the file being executed. 
// argv:  is a null terminated array of character pointers.