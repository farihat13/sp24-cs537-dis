#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("USAGE. <file-name>\n");
        return 0;
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("error opening file\n");
        return 1;
    }

    char buf[80];
    while (fgets(buf, 80, fp) != NULL) {
        printf("%s", buf);
    }
    printf("\n");

    // Move the file position indicator to the beginning of the file
    if (fseek(fp, 0, SEEK_SET) != 0) {
        perror("Error using fseek");
        fclose(fp);
        return 1;  // Exit with an error code
    }
    while (fgets(buf, 80, fp) != NULL) {
        printf("%s", buf);
    }
    printf("\n");

    // Move the file position indicator to the beginning of the file
    if (fseek(fp, 5, SEEK_SET) != 0) {
        perror("Error using fseek");
        fclose(fp);
        return 1;  // Exit with an error code
    }
    while (fgets(buf, 80, fp) != NULL) {
        printf("%s", buf);
    }
    printf("\n");


    fclose(fp);
    return 0;
}