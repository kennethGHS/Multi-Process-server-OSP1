//
// Created by kenneth on 17/11/20.
//
#include "proc_structure.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
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
#include <sys/socket.h>
#include "../modes/file_descriptor_messager.h"
#include "../image_admin/image_receiver.h"

/**
 * This method needs to be called to create all the processes, it inits the list of *numProcesses processes in a list
 * in shared memory, this processes can be accessed by the parend and the other processes
 * it also initiates the semaphore used for the administration of the list
 */
void initList(int processes) {
    
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    numProcesses = mmap(NULL, sizeof(int ), protection, visibility, -1, 0);
    *numProcesses = processes;
    semaphoreList = mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    sem_init(semaphoreList, 1, 0);
    sem_post(semaphoreList);
    if (semaphoreList == SEM_FAILED) {
        perror("Parent  : [sem_open] Failed\n");
        return;
    }
    headList = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
    headList->available = true;
    headList->semaphoreName = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
    headList->semaphore = mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    sem_init(headList->semaphore, 1, 0);
    struct process *initial = headList;
    for (int i = 0; i < *numProcesses; ++i) {
        initial->nextProcess = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
        initial->nextProcess->available = true;
        initial->nextProcess->semaphore = mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        initial->nextProcess->semaphoreName = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
        sem_init(initial->nextProcess->semaphore, 1, 0);
        initial = initial->nextProcess;
    }
    //Meter una creacion de *numProcesses elementos en este lugar
//    //printf("\nLOL\n");
//    headList = NULL;
//    sem_close(sem_id);
}

void print_all_PID() {
    struct process *list = headList;
    while (list->available == false) {
        //printf(" El id es %d \n", list->idProcess);
        list = list->nextProcess;
    }
}

void create_and_execute(int procID) {
    //printf("Creating process %d in forked\n", procID);
    sem_t *proc = semaphoreList;
    int procValue;
    sem_getvalue(proc, &procValue);
    sem_wait(proc);
    struct process *process = headList;
    while (!process->available) {

        process = process->nextProcess;
//        sleep(1);
    }
    process->idProcess = procID;
    sprintf(headList->semaphoreName, "%d", procID);
    process->inExecution = false;
    process->available = false;

    sem_post(proc);
    execute_process(process);
}

void execute_process(struct process *process) {

    while (1 == 1) {
        char *hello = "Hello from server";
        char buffer[1024] = {0};
        long int valread;
        sem_wait(process->semaphore);
        process->socket = receive();
        receive_picture(process->socket );
//        sleep(5);

//        //printf("Fui liberado, el proceso numero %d en la posicion de lista %d \n", getpid(),get_position_list_PID(getpid()));
//        //AQUI DEBE IR EL CODIGO DE SOCKETS
////        sleep(20);
//        valread = read(process->socket, buffer, 1024);
//        //printf("Se leyeron %ld bits",valread);
////        perror("sad");
////        //printf("Se leyo %ld",valread);
//
////        //printf("%s\n", buffer);
////        //printf("Enviado al socket %d \n",process->socket);
//        send(process->socket, hello, strlen(hello), 0);
//        perror("sad");
//
////        //printf("Hello message sent\n");
//        sleep(1);
        close(process->socket);
        process->inExecution = false;
        //printf("Ya termine \n");
    }
}

void release_by_ID(int PID) {
    struct process *head = headList;
    int i = 0;
    while (head->idProcess != PID && i != *numProcesses) {
        i++;
        head = head->nextProcess;
    }
    if (i==*numProcesses){
        return;
    }
    int procValue;
    sem_getvalue(head->semaphore, &procValue);
    sem_post(head->semaphore);
    //printf("Released process %d \n", PID);
}

int release_and_set_available(int socket) {
    struct process *process = headList;
    int i = 0;
    while (process->inExecution && i != *numProcesses) {
        i++;
        process = process->nextProcess;
    }
    if(i==*numProcesses){
        return -1;
    }
    process->socket = socket;
    process->inExecution = true;
//    int procValue;
//    sem_getvalue(process->semaphore, &procValue);
    //printf("El valor de proceso liberado es %d \n", process->idProcess);
    sem_post(process->semaphore);
    return 0;
}

int get_position_list_PID(int PID) {
    //printf("\n PID = %d\n",PID);
    struct process *head = headList;
    int i = 0;
    while (head->idProcess!=PID && i != *numProcesses) {
        i++;
        head = head->nextProcess;
    }
    return i;
}

