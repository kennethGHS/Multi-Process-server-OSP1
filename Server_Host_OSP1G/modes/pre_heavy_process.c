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
    sleep(1);
    while (1) {
        printf("\rAwaiting connection\033[K");
        fflush(stdout);

        fd_set set;
        struct timeval timeout;
        int rv;
        FD_ZERO(&set); /* clear the set */
        FD_SET(server_fd, &set); /* add our file descriptor to the set */

        timeout.tv_sec = 15;
        timeout.tv_usec = 0;

        rv = select(server_fd + 1, &set, NULL, NULL, &timeout);
        if (rv == -1) {
            perror("select"); /* an error accured */
        } else if (rv == 0) {
            printf("\rTimeout occurred (15 seconds)\033[K\n"); /* a timeout occured */
            break;
        } else if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                        (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("\rConnection detected\033[K");
        fflush(stdout);
        while (release_and_set_available(new_socket) == -1) {}
        send_file(new_socket);
        char buf[20];

        fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
        if (read(0, buf, 4) > 0) {
            printf("\n");
            break;
        }
    }
    init_free_process();

}

void create_processes(int processes) {
    for (int i = 0; i < processes; ++i) {
        int id = fork();
        if (id != 0) {
            if (id == -1) {
                printf("Process creation error");
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
    printf("Pre-heavy process server started, press ENTER to stop execution or wait for timeout\n");
    execute_pre_heavy_process(atoi(argv[2]));

}