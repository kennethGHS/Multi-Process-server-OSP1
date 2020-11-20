//
// Created by kenneth on 19/11/20.
//
#include "delegation_mode.h"
#define PORT 8080
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "file_descriptor_messager.h"


void execute_delegation(int processes){
    initList(processes);
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    create_processes(processes);
    while (1) {
        printf("Esperando \n");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
//        printf("Detected connection\n");
        release_and_set_available(new_socket);
        send_file(new_socket);
        close(new_socket);



//        int id = fork();
//        if (id!=0){
//            continue;
//        }
    }

}
void create_processes(int processes) {
    for (int i = 0; i < processes; ++i) {
        int id = fork();
        if (id != 0) {
            if (id == -1) {
                printf("Error a la hora de creacion");
            }
            continue;
        }

        printf("Creating process %d\n", i);
        create_and_execute(getpid());
        return;
    }
}