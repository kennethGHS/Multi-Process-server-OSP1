//
// Created by kenneth on 19/11/20.
//

#include "file_descriptor_messager.h"
/**
 * All of this code was recovered from this stack overflow question:
https://stackoverflow.com/questions/28003921/sending-file-descriptor-by-linux-socket/
 *and adapted to what the project needed, some minor modifications like the closing of some sockets was deleted
 *
 */



/**
 * Sends the file descriptor to a child process
 * @param fileDescriptor
 */
void send_file(int fileDescriptor) {

    int sock = sv[0];
    int fd = fileDescriptor;
    send_from_father(sock, fd);

    close(fd);
    nanosleep(&(struct timespec) {.tv_sec = 0, .tv_nsec = 5000000}, 0);
}
/**
 * Receives thje file descriptor from a parent
 * @return
 */
int receive() {

    int sock = sv[1];
    nanosleep(&(struct timespec) {.tv_sec = 0, .tv_nsec = 5000000}, 0);
    int fd = receive_from_father(sock);
    return fd;
}
/**
 * Configures the communication between processes
 */
void configure_comunication() {
    if (socketpair(AF_UNIX, SOCK_DGRAM, 0, sv) != 0)
        printf("Failed to create Unix-domain socket pair\n");
}
/**
 * Sends a file descriptor to a child process
 * @param socket socket to send the file to
 * @param fd file descriptor
 */
static
void send_from_father(int socket, int fd)  // send fd by socket
{
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(fd))];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = {.iov_base = "ABC", .iov_len = 3};

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    *((int *) CMSG_DATA(cmsg)) = fd;

    msg.msg_controllen = CMSG_SPACE(sizeof(fd));

    if (sendmsg(socket, &msg, 0) < 0)
        printf("Failed to send message\n");
}
/**
 * Receives the file descriptor from its father
 * @param socket socket to read the fd from
 * @return
 */
static
int receive_from_father(int socket)  // receive fd from socket
{
    struct msghdr msg = {0};

    char m_buffer[256];
    struct iovec io = {.iov_base = m_buffer, .iov_len = sizeof(m_buffer)};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char c_buffer[256];
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);

    if (recvmsg(socket, &msg, 0) < 0)
        printf("Failed to receive message\n");

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    unsigned char *data = CMSG_DATA(cmsg);

    int fd = *((int *) data);
    printf("\rExtracted fd %d\033[K", fd);

    return fd;
}
