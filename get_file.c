#include "get_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *get_file(char *fileName) {
    // open file
    FILE *fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("File %s does not exist\n", fileName);
        return NULL;
    }

    // set file pointer at end of file
    if (fseek(fp, 0, SEEK_END) != 0) {
        printf("fseek failed.\n");
        return NULL;
    }

    // get file size
    int fileSize = ftell(fp);

    if (fileSize == -1) {
        printf("ftell failed.\n");
        return NULL;
    }

    // set file pointer at start of file
    if (fseek(fp, 0, SEEK_SET) != 0) {
        printf("fseek failed.\n");
        return NULL;
    }

    // malloc buffer
    char *buffer = (char *)malloc(fileSize + 1);

    // read contents of file to buffer
    size_t bytesRead = fread(buffer, fileSize, 1, fp);

    if (bytesRead != (size_t)fileSize) {
        printf("fread failed.\n");
        fclose(fp);
        free(buffer);
        return NULL;
    }

    // null terminate buffer and close file
    buffer[fileSize] = '\0';
    fclose(fp);

    // return buffer
    return buffer;
}
