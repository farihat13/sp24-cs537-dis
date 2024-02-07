#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
    int file_desc = open("dummy.txt", O_WRONLY | O_APPEND);
    dup2(file_desc, 1);
    printf("this line will be printed in dummy.txt\n");
    return 0;
}
