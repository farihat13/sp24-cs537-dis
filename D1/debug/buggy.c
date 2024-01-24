#include <stdio.h>

struct Data
{
    int x;
};

int main(int argc, char *argv[])
{
    printf("first line of main in %s\n", argv[0]);
    int a = 5;
    int b = 100;
    printf("a = %d, b = %d, a*b = %d\n", a, b, a*b);
    
    struct Data *p = NULL;
    printf("%d\n", p->x);
}

