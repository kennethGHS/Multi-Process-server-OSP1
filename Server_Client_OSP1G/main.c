#include <stdio.h>
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
#include <pthread.h>

#define PORT 8080
#define PORT 8080

int executeClient(){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    printf("Esperando");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
//    sleep(2);
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("Leido\n");
    printf("%s\n",buffer );
    close(sock);
    exit(0);
    return 0;
}
int main(int argc, char const *argv[])
{
    int ids[100];
    for (int i = 0; i < 50; ++i) {
        int id = fork();
        if (id!=0){
            if (id==-1){
                printf("Failed creating process \n");
            }
            continue;
        }

        printf("Numero %d\n",i);
        executeClient();
    }
    int wpid;
    int status;
    while ((wpid = wait(&status)) > 0);
}