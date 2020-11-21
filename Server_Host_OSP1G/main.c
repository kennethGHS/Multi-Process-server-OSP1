#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
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
#include "modes/serial_mode.h"
#include "modes/file_descriptor_messager.h"
#include "modes/continuous_mode.h"
#include "image_admin/image_receiver.h"
static
void wyslij(int socket, int fd)  // send fd by socket
{
    struct msghdr msg = { 0 };
    char buf[CMSG_SPACE(sizeof(fd))];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = "ABC", .iov_len = 3 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    *((int *) CMSG_DATA(cmsg)) = fd;

    msg.msg_controllen = CMSG_SPACE(sizeof(fd));

    if (sendmsg(socket, &msg, 0) < 0)
        printf("Failed to send message\n");
}

static
int odbierz(int socket)  // receive fd from socket
{
    struct msghdr msg = {0};

    char m_buffer[256];
    struct iovec io = { .iov_base = m_buffer, .iov_len = sizeof(m_buffer) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char c_buffer[256];
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);

    if (recvmsg(socket, &msg, 0) < 0)
        printf("Failed to receive message\n");

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);

    unsigned char * data = CMSG_DATA(cmsg);

    printf("About to extract fd\n");
    int fd = *((int*) data);
    printf("Extracted fd %d\n", fd);

    return fd;
}


void releasingTest(){
    initList(200);

    for (int i = 0; i < 200; ++i) {
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
        return ;
    }
    int wpid,status;
    while (1==1){
        int num, nitems;
        printf("\nPor favor introducir Id del numero:\n");
        nitems = scanf("%d", &num);
        if (nitems == EOF) {
            /* Handle EOF/Failure */
        } else if (nitems == 0) {
            /* Handle no match */
        } else {
            release_by_ID(num);
            printf("Got %d\n", num);
        }
    }
    while ((wpid = wait(&status)) > 0);
    print_all_PID();
//executeServer();
}
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
    configureImageReceiver();
//    executeServer();
configure_comunication();
execute_serial();
//execute_continuous();
//    execute_delegation(100);
//releasingTest();

}

