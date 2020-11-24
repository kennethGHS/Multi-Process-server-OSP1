//
// Created by kenneth on 20/11/20.
//

#include "sequential.h"

void execute_sequential() {

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 100) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        printf("Esperando \n");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Detected connection\n");
        receive_picture(new_socket);
    }
}

int main(int argc, char const *argv[]) {

    if (argc != 2) {
        printf("Not enough arguments. Usage is: sequential <port>\n");
        return 1;
    }
    PORT = atoi(argv[1]);

    configureImageReceiver("./sequential_images/"); // ESTO SIEMPRE SE EJECUTA
    configure_comunication(); // ESTO SIEMPRE SE EJECUTA
    execute_sequential();
}