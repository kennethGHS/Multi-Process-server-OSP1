//
// Created by kenneth on 19/11/20.
//

#ifndef SERVER_HOST_OSP1G_DELEGATION_MODE_H
#define SERVER_HOST_OSP1G_DELEGATION_MODE_H


#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "file_descriptor_messager.h"
#include "../proc_structures/proc_structure.h"

#define PORT 8080


void execute_delegation(int processes);

void create_processes(int processes);

#endif //SERVER_HOST_OSP1G_DELEGATION_MODE_H
