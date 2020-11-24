//
// Created by kenneth on 20/11/20.
//

#ifndef SERVER_HOST_OSP1G_SEQUENTIAL_H
#define SERVER_HOST_OSP1G_SEQUENTIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "file_descriptor_messager.h"

#include "../image_admin/image_receiver.h"

int PORT;

void execute_sequential();

#endif //SERVER_HOST_OSP1G_SEQUENTIAL_H
