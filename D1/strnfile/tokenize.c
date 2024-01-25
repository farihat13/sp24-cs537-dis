#include <stdio.h>
#include <string.h>

int main() {
    char str[] = "Hello, World! This is a sample string that is just long. ";
    char delimiters[] = " ,.!"; // Space, comma, and exclamation mark as delimiters

    char* word = "is"; // We are searching for this word

    // Tokenize the string using strtok
    char *token = strtok(str, delimiters);

    // Iterate through the tokens
    while (token != NULL) {
        printf("Token: %s\n", token);

        if (strcmp(token, word) == 0) {
            printf("\t\tFound it!\n");
            // break; // Try commenting it out
        }

        // Get the next token
        token = strtok(NULL, delimiters);
    }

    return 0;
}
