
#include <stdio.h>
#include <unistd.h>

int main()
{
    int p[2];
	if (pipe(p) < 0)
		return 1;
    // write to pipe
	write(p[1], "hello", 5);
	write(p[1], "world", 5);

    // read from pipe
    char inbuf[100];
    read(p[0], inbuf, 100);
    // print
    printf("%s\n", inbuf);

    // close 
    close(p[1]); // try commenting it out.

    // read from pipe
    char inbuf2[100];
    read(p[0], inbuf2, 100);
    // print
    printf("%s\n", inbuf2);
	return 0;
}
