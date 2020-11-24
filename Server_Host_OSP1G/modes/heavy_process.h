//
// Created by kenneth on 20/11/20.
//

#ifndef SERVER_HOST_OSP1G_HEAVY_PROCESS_H
#define SERVER_HOST_OSP1G_HEAVY_PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <zconf.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../image_admin/image_receiver.h"
#include "file_descriptor_messager.h"

int PORT;

void execute_heavy_process();

#endif //SERVER_HOST_OSP1G_HEAVY_PROCESS_H
