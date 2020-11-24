//
// Created by kenneth on 20/11/20.
//
#include <pthread.h>
#include <ifaddrs.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/**
 * Method used to get the extension of a file and verify its either png or jpg or jpeg
 * @param filename The direction to the file
 * @return 1 for a png, 2 for a jpeg or jpg and -1 if neither
 */
int get_image_type(char *filename) {

    int filenamesize = strlen(filename);
    char fileextension[] = {filename[filenamesize - 3], filename[filenamesize - 2], filename[filenamesize - 1], '\0'};
    char pngExtension[] = "PNGpng";
    char jpgExtension[] = "JPGjpgJPEGjpeg";
    if (strstr(pngExtension, fileextension) != NULL) {
        return 1;
    }
    if (strstr(jpgExtension, fileextension) != NULL) {
        return 2;
    } else {
        return -1;
    }
}

int check_file_existence(char *filepath) {
    FILE *fp;
    if ((fp = fopen(filepath, "rb")) == NULL) {
        return -1;
    } else {
        fclose(fp);
        return 1;
    }
}

