//
// Created by kenneth on 19/11/20.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
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
#include "file_descriptor_messager.h"
#define PORT 8080
void send_file(int fileDescriptor){
//    printf("Parent at work\n");
//    close(sv[1]);
    int sock = sv[0];

    int fd = fileDescriptor;
    wyslij(sock, fd);

    close(fd);
    //SI DA PROBLEMAS REVISAR ESTO
    nanosleep(&(struct timespec){ .tv_sec = 0, .tv_nsec = 5000000}, 0);
//    printf("Parent exits\n");
}
int receive(){
//    printf("Child at play\n");
//    close(sv[0]);
    int sock = sv[1];
    nanosleep(&(struct timespec){ .tv_sec = 0, .tv_nsec = 5000000}, 0);
    int fd = odbierz(sock);
    return fd;
//    printf("Read %d!\n", fd);
//    char buffer[256];
//    ssize_t nbytes;
//    while ((nbytes = read(fd, buffer, sizeof(buffer))) > 0)
//        write(1, buffer, nbytes);
//    printf("Done!\n");
//    close(fd);
}
 void configure_comunication(){
    if (socketpair(AF_UNIX, SOCK_DGRAM, 0, sv) != 0)
        printf("Failed to create Unix-domain socket pair\n");
}
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