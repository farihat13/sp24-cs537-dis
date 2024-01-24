#include<stdio.h>

int main(int argc, char* argv[]){ // 
    
    printf("program <%s> running\n", argv[0]); 
    
    if (argc > 1) 
        printf("1st arg: %s\n", argv[1]);
    else // if argc == 1
        printf("hello world\n");
    
    int a = 5;
    //int a = 5/0; // comment in and compile with -Werror, -Wall
}
                
