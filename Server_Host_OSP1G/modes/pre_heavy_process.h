//
// Created by kenneth on 19/11/20.
//

#ifndef SERVER_HOST_OSP1G_PRE_HEAVY_PROCESS_H
#define SERVER_HOST_OSP1G_PRE_HEAVY_PROCESS_H


#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "file_descriptor_messager.h"
#include "../proc_structures/proc_structure.h"

int PORT;

void execute_pre_heavy_process(int processes);

void create_processes(int processes);

#endif //SERVER_HOST_OSP1G_PRE_HEAVY_PROCESS_H
