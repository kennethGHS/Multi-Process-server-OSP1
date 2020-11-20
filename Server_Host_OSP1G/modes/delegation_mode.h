//
// Created by kenneth on 19/11/20.
//

#ifndef SERVER_HOST_OSP1G_DELEGATION_MODE_H
#define SERVER_HOST_OSP1G_DELEGATION_MODE_H
#include "../proc_structures/proc_structure.h"
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
void execute_delegation(int processes);
void create_processes(int processes);
#endif //SERVER_HOST_OSP1G_DELEGATION_MODE_H
