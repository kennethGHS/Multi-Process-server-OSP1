//
// Created by kenneth on 20/11/20.
//

#ifndef SERVER_HOST_OSP1G_IMAGE_RECEIVER_H
#define SERVER_HOST_OSP1G_IMAGE_RECEIVER_H

#include <semaphore.h>

void configureImageReceiver();
int receive_picture(int socket);
void check_directories();
int increase_num_images();
char* concact_dir(char * filename);
char *get_file_name(char *file_directory);
int get_last_index(char *file_directory);
int * numImages;
sem_t * numImagesSemaphore;
int remove_image(char * imageName);
int calcule_num_files();
void execute_filter(char * filename); // En esta meten el codigo del filtro
#endif //SERVER_HOST_OSP1G_IMAGE_RECEIVER_H
