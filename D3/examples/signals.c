#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

// Handler for SIGINT, caused by Ctrl-C at keyboard
void my_handler_sigint(int sig)
{
	printf("\nCaught signal %d\n", sig);
    exit(0);
}
int main()
{
	signal(SIGINT, my_handler_sigint);
	while (1) {
        printf("c\n");
        sleep(1);
    }
	return 0;
}