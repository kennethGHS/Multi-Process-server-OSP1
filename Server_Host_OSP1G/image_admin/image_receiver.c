//
// Created by kenneth on 20/11/20.
//

#include "image_receiver.h"

//TODO make different for each server
char images_path[1024];

void configureImageReceiver(char *path) {
    strncpy(images_path, path, strlen(path));
    check_directories();

    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    numImagesSemaphore = mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    sem_init(numImagesSemaphore, 1, 1);
    numImages = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
    *numImages = 0;
}

int increase_num_images() {
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
    if (stat(images_path, &st) == -1) {
        mkdir(images_path, 0700);
    }
    char cmd[300] = "exec rm -r ";
    system(strcat(strcat(cmd, images_path), "*"));
}

/**
 * Concatenates to an specific directory, either not trusted or TEMP
 * @param filename
 * @param directory
 * @return the directory with the filename
 */
char *concact_dir(char *filename) {
    char *result = malloc(strlen(filename) + strlen(images_path) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, images_path);
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
        random += rand() + getpid();
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
    perror("open");

    int sumatoria = 0;
    long int writtenBytes;
    memset(buffer, 0, sizeof(buffer));
    int bytes_toRead = 1024;
    while (len != 0) {
        long int read_b;
        read_b = read(socket, buffer, bytes_toRead);

        perror("read");
        printf("Read %ld \n", read_b);
        sumatoria += read_b;
        if (read_b == 0) {
            printf("Lol read fue 0\n");
        }
        len -= read_b;
        while (read_b != 0) {
            writtenBytes = fwrite(buffer, 1, read_b, fp);
            perror("sad");
            sumatoriaEscrito += writtenBytes;
            read_b -= writtenBytes;
            printf("written %ld \n", writtenBytes);
        }
        if (len < 1024) {
            bytes_toRead = len;
        }
    }
    printf("Se sumaron %d deberia ser %d sum de escrito %d con nombre %s\n", sumatoria, lencopy, sumatoriaEscrito,
           finalFilename);
    fclose(fp);
    execute_filter(finalFilename);

//    execute_filter(finalFilename);
    if (calcule_num_files() >= 102) {
        remove_image(finalFilename);
    }
    free(finalFilename);
//    sleep(1);

    send(socket, "Confirmado", sizeof("confirmado"), 0);

    close(socket);
}


int remove_image(char *imageName) {
    char fileToDelete[200];
    fileToDelete[0] = '\0';
    strcpy(fileToDelete, "exec rm -r ");
    strcat(fileToDelete, imageName);
    system(fileToDelete);

}

int calcule_num_files() {
    int file_count = 0;
    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(images_path); /* There should be error handling after this */
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            file_count++;
        }
    }
    closedir(dirp);
    return file_count;
}

void execute_filter(char *filename) {
    char cmd[200];
    strcpy(cmd, "python3 ./image_admin/sobel.py ");
    int x = system(strcat(cmd, filename));

}