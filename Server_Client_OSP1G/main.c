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
#include <omp.h>
#include "Validations/image_validations.h"
#define PORT 8080
#define PORT 8080
/**
 * Method that sends a picture to the host
 * @param filename teh path to the file to send
 * @param ip the ip of the host
 * @return 0 if everything went right -1 otherwise
 */
int execute_server_client( char* filename){
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //"127.0.0.1"
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\n Error Cannot connect to ip\n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    //Envia el filename
//CAMBIAR!!! no ocupo enviar el filename ya que generare uno random pero lo utilizare como una señal de que iniciare
// el envio de una imagen

    send(sock , filename , strlen(filename)+1 , 0 );
    printf("FilenameSend\n");
    //Lee la confirmacion de parte del servidor
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    FILE * fp = fopen(filename,"rb");
    //Ocupo ver el filesize sea mayor a 1024
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    //convierto el int a un string para enviarlo
    char str_num[50];
    sprintf(str_num, "%d", sz);
    send(sock,str_num,strlen(str_num)+1,0);
    long int sentBytes=0;
    size_t readBytes = 0;
    printf("The size is %d \n",sz);
    //ciclo de enviar la imagen
    valread = read( sock , buffer, 1024);
    memset(buffer,0,sizeof(buffer));

    while (sz!=0){
        readBytes =fread(buffer,sizeof(char),1024,fp);
        perror("sad");
        while (readBytes!=0){
            sentBytes = send(sock,buffer,1024,0);
            perror("sad");
            readBytes-=sentBytes;
            sz-=sentBytes;
        }
        if(sz<1024){
            readBytes =fread(buffer,sizeof(char),sz,fp);
            sentBytes = send(sock,buffer,1024,0);
            break;
        }
    }
    //espera a confirmacion de que se termino de procesar
//    valread = read( sock , buffer, 1024);
    return 1 ;

//    if (sz<=1024){
//
//
//
//
//        fread(buffer,sz,1,fp);
//        send(sock,buffer,sz,0);
//        return 0;
//    } else{
//        while (sz!=0){
//            sleep(0.001);
//            fread(buffer,1024,1,fp);
//            sz-=1024;
//            sleep(0.001);
//            send(sock,buffer,1024,0);
//            if (sz<=1024){
//                fread(buffer,sz,1,fp);
//                send(sock,buffer,sz,0);
//                printf("The file has been successfully sent\n");
//                fclose(fp);
//                close(sock);
//                return 0;
//            }
//        }
//
//    }
//    printf("Se ha terminado de enviar la imagen \n");
//    return 0;
}
//
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
    printf("Leido en proceso %d con socket %d\n",getpid(),sock);
    printf("\n Se llego: %s\n",buffer );
    close(sock);
    return 0;
}
int main(int argc, char const *argv[])
{

    int num_threads = 2;
    char * filename = "/home/kenneth/Pictures/Result.png";
    omp_set_num_threads(num_threads);
//    execute_server_client(filename);3114
    double time_all = omp_get_wtime(); // el que se tarda por solicitud
    double times = 0;
    double times_per_thread = 0;
#pragma omp  parallel
    {
        double timeInit = omp_get_wtime();
        int num_images = 10; // aqui hay que poner los que diga el usuario
        while (num_images != 0) {
            double init_Time = omp_get_wtime();
            execute_server_client(filename);
            num_images--;
#pragma omp critical
            times += (omp_get_wtime() - init_Time);
        }
#pragma omp critical
        times_per_thread+=(omp_get_wtime() - timeInit);
    }

    double time_all2 = omp_get_wtime() - time_all;


}
