#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <omp.h>
#include <stdlib.h>
#include "Validations/image_validations.h"


/**
 * Method that sends a picture to the host
 * @param filename the path to the file to send
 * @param ip the ip of the host
 * @return 0 if everything went right 1 otherwise
 */
int execute_server_client(char *ip_address, int port, char *filename) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        printf("\n Error: Cannot connect to ip\n");
        return 1;
    }
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return 1;
    }

    //Envia el filename
    send(sock, filename, strlen(filename) + 1, 0);
    printf("Filename sent\n");
    //Lee la confirmacion de parte del servidor
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    FILE *fp = fopen(filename, "rb");
    //Ocupo ver el filesize sea mayor a 1024
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    //convierto el int a un string para enviarlo
    char str_num[50];
    sprintf(str_num, "%d", sz);
    send(sock, str_num, strlen(str_num) + 1, 0);
    long int sentBytes = 0;
    size_t readBytes = 0;
    printf("The size is %d \n", sz);
    //ciclo de enviar la imagen
    valread = read(sock, buffer, 1024);
    memset(buffer, 0, sizeof(buffer));

    while (sz != 0) {
        readBytes = fread(buffer, sizeof(char), 1024, fp);
        while (readBytes != 0) {
            sentBytes = send(sock, buffer, 1024, 0);
            readBytes -= sentBytes;
            sz -= sentBytes;
        }
        if (sz < 1024) {
            readBytes = fread(buffer, sizeof(char), sz, fp);
            sentBytes = send(sock, buffer, 1024, 0);
            break;
        }
    }
    // Wait for finish confirmation
    valread = read( sock , buffer, 1024);
    return 0;
}


int main(int argc, char const *argv[]) {

    if (argc != 6) {
        printf("Not enough arguments. Usage is: client <ip address> <port> <filename> <n-threads> <n-cycles>\n");
        return 1;
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);
    char *filename = argv[3];
    int num_threads = atoi(argv[4]);
    int num_images = atoi(argv[5]);

    if (port == 0 || num_threads == 0 || num_images == 0) {
        printf("Invalid arguments. Usage is: client <ip address> <port> <filename> <n-threads> <n-cycles>\n");
        return 1;
    }

    if(check_file_existence(filename) == -1 || get_image_type(filename) == -1){
        printf("Invalid image path or invalid format. Format must be .jpeg, .jpg or .png\n");
        return 1;
    }

    omp_set_num_threads(num_threads);
    double time_all = omp_get_wtime(); // el que se tarda por solicitud
    double times = 0;
    double times_per_thread = 0;
#pragma omp parallel
    {
        double timeInit = omp_get_wtime();
        for(int i = 0; i < num_images; i++) {
            printf("Thread: %d Cycle: %d\n", omp_get_thread_num(), i);
            double init_Time = omp_get_wtime();
            execute_server_client(ip_address, port, filename);
#pragma omp critical
            times += (omp_get_wtime() - init_Time);
        }
#pragma omp critical
        times_per_thread += (omp_get_wtime() - timeInit);
    }

    double time_all2 = omp_get_wtime() - time_all;


}
