#include <stdio.h>
#include <errno.h>

int main() {
    FILE *file = fopen("a.txt", "r"); // try with a non-existent file
    if (file == NULL) {
        perror("Error opening file");    
    } else {
        printf("File opened successfully!\n");
        int ch;
        // Read the file character by character
        while ((ch = fgetc(file)) != EOF) {
            printf("%c", ch);   
        }
        printf("\n");
        // Remember to close the file when done
        fclose(file);
    }
    return 0;
}
