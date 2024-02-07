// execDemo.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
    char *args[] = {"./fork", NULL};
    execv(args[0], args);
    // will not execute unless error
    printf("Ending-----");
    return 0;
}

// With execv(), the first argument is a path to the executable.
// With execvp(), the first argument is a filename. 
// It must be converted to a path before it can used. This involves looking for the filename in all of the directories in the PATH environment variable.

