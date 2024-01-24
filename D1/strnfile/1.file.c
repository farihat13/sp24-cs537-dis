#include <stdio.h>
#include <errno.h>

int main() {
    FILE *file = fopen("b.txt", "rw");
    if (file == NULL) {
        perror("Error opening file");    
        // Alternatively, you can check the value of errno directly
        if (errno == ENOENT) {
            printf("File not found.\n");
        } else {
            printf("Unknown error.\n");
        }
    } else {
        printf("File opened successfully!\n");
        int ch;
        while ((ch = fgetc(file)) != EOF) {
            printf("%c", ch);   
        }
        printf("\n");
        // Remember to close the file when done
        fclose(file);
    }

    return 0;
}
