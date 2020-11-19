#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include "proc_structures/proc_structure.h"
#define PORT 8080
int executeServer(){
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

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
    while (1) {
        printf("Esperando \n");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }        printf("Esperando mensaje \n");
        int id = fork();
        if (id!=0){
            continue;
        }
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        printf("Enviado \n");
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        close(new_socket);
        exit(0);
    }
    return 0;
}
int main(int argc, char const *argv[])
{
    initList();

    for (int i = 0; i < 100; ++i) {
        int id =fork();
        if (id!=0){
            if (id==-1){
                printf("Error a la hora de creacion");
            }
            continue;
        }
        if (id==-1){
            printf("Error a la hora de creacion");
        }
        printf("Creating process %d\n",i);
        create_and_execute(getpid());
        return 0;
    }
    int wpid,status;
    while ((wpid = wait(&status)) > 0);
    print_all_PID();
//executeServer();
}

