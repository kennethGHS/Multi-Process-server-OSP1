//
// Created by kenneth on 19/11/20.
//
#include "pre_heavy_process.h"

void execute_pre_heavy_process(int processes) {
    initList(processes);
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
    create_processes(processes);
    while (1) {
        printf("Esperando \n");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Detected connection\n");
        while (release_and_set_available(new_socket) == -1) {

        }
        send_file(new_socket);
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

int main(int argc, char const *argv[]) {

    if (argc != 3) {
        printf("Not enough arguments. Usage is: sequential <port> <N-processes>\n");
        return 1;
    }
    PORT = atoi(argv[1]);

    configureImageReceiver("./pre_h_process_images/"); // ESTO SIEMPRE SE EJECUTA
    configure_comunication(); // ESTO SIEMPRE SE EJECUTA
    execute_pre_heavy_process(atoi(argv[2]));

}