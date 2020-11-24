//
// Created by kenneth on 19/11/20.
//

#ifndef SERVER_HOST_OSP1G_FILE_DESCRIPTOR_MESSAGER_H
#define SERVER_HOST_OSP1G_FILE_DESCRIPTOR_MESSAGER_H


#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#define PORT 8080

int sv[2];

static void wyslij(int socket, int fd);

static int odbierz(int socket);

void send_file(int fileDescriptor);

void configure_comunication();

int receive();

#endif //SERVER_HOST_OSP1G_FILE_DESCRIPTOR_MESSAGER_H
