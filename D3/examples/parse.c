#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <errno.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h>
int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        write(STDOUT_FILENO, argv[i], strlen(argv[i]));
        write(STDOUT_FILENO, "\n", strlen("\n"));
    }
    
    printf("waiting for a line from you\n");
    char line[100];
    fgets(line, 100, stdin);
    write(STDOUT_FILENO, line, strlen(line));

    
    printf("\ntokenizing your line: %s\n", line);
    char *p;
    p = strtok (line, " -/");
    while (p != NULL)
    {
        printf("token: '%s'\n", p); 
        p = strtok (NULL, " -/");
    }

    char str[] = "ls - prints the files/dirs";
    printf("str '%s'\n", str);
    char *copy = strdup(str);
    printf("copy '%s'\n", copy);
    char *word = strndup(str, 3);
    printf("str '%s'\n", word);
}

// The function strcpy() will not allocate the memory space to copy. 
// A pointer to the string to copy and a pointer to place to copy it 
// to should be given. 
// The function strdup() will occupy / grab itself the memory space 
// for copying the string to.