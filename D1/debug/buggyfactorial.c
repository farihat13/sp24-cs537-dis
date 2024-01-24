#include<stdio.h>

int main(){

    int n = 4;
    int i, f;
    
    for (i=1;i<=n;i++){
        f = f*i;
    }
    printf("%d! = %d\n", n, f);
}