//
// Created by kenneth on 20/11/20.
//

#include "image_receiver.h"
#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>

void configureImageReceiver() {
    check_directories();
    system("exec rm -r ../Images/*");
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    numImagesSemaphore = mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    sem_init(numImagesSemaphore, 1, 1);
    numImages = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    *numImages = 0;
}

int increase_num_images() {
    //printf("LOL semaforo\n");
    sem_wait(numImagesSemaphore);
    //printf("cantidad de num e iamgenes %d \n",*numImages);
        (*numImages)++;
        sem_post(numImagesSemaphore);
        return 0;
}

/**
 * Check if the directories necessary for the image processing procedures are present
 */
void check_directories() {
    struct stat st = {0};
    if (stat("../Images/", &st) == -1) {
        mkdir("../Images/", 0700);
    }
}

/**
 * Concatenates to an specific directory, either not trusted or TEMP
 * @param filename
 * @param directory
 * @return the directory with the filename
 */
char *concact_dir(char *filename) {
    char *result = malloc(strlen(filename) + strlen("../Images/") + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, "../Images/");
    strcat(result, filename);
    return result;
}

/**
 * Gets last directory index
 * @param file_directory the dir of a image
 * @return the index of the last / or \\
 */
int get_last_index(char *file_directory) {
    int filename_size = strlen(file_directory);
    int last_slash_index = -1;
    for (int i = 0; i < filename_size; ++i) {
        if (file_directory[i] == '/' || file_directory[i] == '\\') {
            last_slash_index = i;
        }
    }
    return last_slash_index;

}

/**
 * Extracts the filename out of an directory
 * @param file_directory directory to a file
 * @return the filename of the file in the directory
 */
char *get_file_name(char *file_directory) {
    int last_slash_index = get_last_index(file_directory);
    int size_filename = sizeof(char) * (strlen(file_directory) - last_slash_index + 1);
    char *filename = malloc(size_filename);
    filename[size_filename - 1] = '\0';
    last_slash_index += 1;
    int i = 0;
    while (i < size_filename) {
        filename[i] = file_directory[last_slash_index + i];
        i++;
    }
    return filename;
}

int receive_picture(int socket) {
    increase_num_images();
    //printf("Receiven picture\n");
    int buffSize = 1024;
    char buffer[buffSize];
    long int readBytes = 0;
    srand(getpid() + *numImages);
    //to generate an even randomner number;
    long int random = rand();
    char filename[200];
    for (int i = 0; i < 10; ++i) {
        random += rand() +getpid();
    }
    //AQUI se consigue el nombre del archivo
    read(socket, buffer, buffSize);
    //printf("\n He recibido \n");
    char *filenameReceived = get_file_name(buffer);
    sprintf(filename, "%ld", random);
    //hay que dealocar el nombre de la imagen
    strcat(filename, filenameReceived);
    free(filenameReceived);
    //AQUI deberia revisar los directoriso
    send(socket, "Confirmado", sizeof("confirmado"), 0);
    int sumatoriaEscrito = 0;
    read(socket, buffer, buffSize);
    //consigo el largo del archivo
    send(socket, "Confirmado", sizeof("confirmado"), 0);

    int len = atoi(buffer);
    int lencopy = len;
    char *finalFilename = concact_dir(filename);
    FILE *fp = fopen(finalFilename, "wb");
    int sumatoria=0;
    long int writtenBytes;
    memset(buffer,0,sizeof(buffer));
    while (len!=0){
        long int read_b;
        read_b = read(socket, buffer, 1024);
        len-=read_b;
        writtenBytes = fwrite(buffer, 1, read_b, fp);
        sumatoriaEscrito+=writtenBytes;
        sumatoria+=read_b;
        read_b-=writtenBytes;
        while (read_b!=0){
            read_b-=fwrite(buffer, 1, read_b, fp);
        }
        if (len<1024){
            read_b = read(socket, buffer, len);
            sumatoria+=len;
            len-=read_b;
            writtenBytes = fwrite(buffer, 1, len, fp);
            sumatoriaEscrito+=writtenBytes;
            if (sumatoriaEscrito<sumatoria){
                read_b = read(socket, sumatoria-sumatoriaEscrito, len);
                fwrite(buffer, 1, sumatoria-sumatoriaEscrito, fp);
            }
            printf("len restante del proceso %s es %d",finalFilename,len);
            break;
        }
//        if (len<1024){
//            read_b = read(socket, buffer, 1024);
//            fwrite(buffer, len, 1, fp);
//
//            break;
//        }
    }
    printf("Se sumaron %d deberia ser %d sum de escrito %d con nombre %s\n",sumatoria,lencopy,sumatoriaEscrito,finalFilename);
    //printf("Written %d\n",sumatoria);
    //printf("Also the filename was %s",finalFilename);
    //printf("Tambien el numero de proceso fue %d",getpid());
    close(fp);
//    while (len != 0) {
//        readBytes = read(socket, buffer, buffSize);
//        len -= readBytes;
//        fwrite(buffer, sizeof(char), buffSize, fp);
//        long int writtenBytes = 0;
////        while (readBytes == 0 && len != 0) {
////            readBytes = len;
////            writtenBytes = fwrite(buffer, sizeof(char), buffSize, fp);
////            len -= writtenBytes;
////        }
////        while (readBytes != 0) {
////            writtenBytes = fwrite(buffer, sizeof(char), buffSize, fp);
////            readBytes -= writtenBytes;
////        }
//        if (len < 1024) {
//            readBytes = read(socket, buffer, buffSize);
//            writtenBytes = fwrite(buffer, sizeof(char), buffSize, fp);
//            break;
//        }
//    }
//    close(fp);
//    if (*numImages == 100) {
//        remove_image(finalFilename);
//        close(socket);
//        free(finalFilename);
//        return 0;
//
//    } else {
//        int isGood = increase_num_images();
//        if (isGood == -1) {
//            remove_image(finalFilename);
//            free(finalFilename);
//            close(socket);
//            return 0;
//        }
//    }
//    execute_filter(finalFilename);
    execute_filter(finalFilename);
    if (calcule_num_files()>=102){
        remove_image(finalFilename);
    }
    free(finalFilename);

    close(socket);
}


int remove_image(char *imageName) {
    char fileToDelete[200];
    fileToDelete[0] ='\0';
    strcpy(fileToDelete,"exec rm -r ");
    strcat(fileToDelete,imageName);
    system(fileToDelete);

}

int calcule_num_files() {
    int file_count = 0;
    DIR * dirp;
    struct dirent * entry;

    dirp = opendir("../Images/"); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}

void execute_filter(char *filename) {}