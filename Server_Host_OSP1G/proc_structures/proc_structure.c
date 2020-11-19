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
/**
 * This method needs to be called to create all the processes, it inits the list of 100 processes in a list
 * in shared memory, this processes can be accessed by the parend and the other processes
 * it also initiates the semaphore used for the administration of the list
 */
void initList() {


    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
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

    struct process *initial = headList;
    for (int i = 0; i < 100; ++i) {
        initial->nextProcess = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
        initial->nextProcess->available = true;
        initial->semaphoreName = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
        initial = initial->nextProcess;
    }
    //Meter una creacion de 100 elementos en este lugar
//    printf("\nLOL\n");
//    headList = NULL;
//    sem_close(sem_id);
}

void print_all_PID() {
    struct process *list = headList;
    while (list->available == false) {
        printf(" El id es %d \n", list->idProcess);
        list = list->nextProcess;
    }
}

void create_and_execute(int procID) {
    printf("Creating process %d in forked\n", procID);
    sem_t *proc = semaphoreList;
    int procValue;
    sem_getvalue(proc, &procValue);
    printf("El valor de smeaforo es %d \n", procValue);
    sem_wait(proc);
    sem_getvalue(proc, &procValue);

    printf("El valor de smeaforo es %d \n", procValue);

    printf("Sleeping \n");
//    sleep(2);


//    printf("\nEntrando al wait \n");
//    int cosa;
//    sem_getvalue(proc,&cosa);
//    printf("El valor es %d\n",cosa);
//    printf("\nsaliendo al wait \n");
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    struct process *process = headList;
    while (!process->available) {
//        printf("LOL funbque %d\n",process->idProcess);
//print_all_PID();
        process = process->nextProcess;
//        sleep(1);
    }
    process->idProcess = procID;
    sprintf(headList->semaphoreName, "%d", procID);
    process->semaphore =mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
    sem_init(process->semaphore, 1, 0);
    process->inExecution = false;
    process->available = false;
//    if (headList == NULL) {
//        printf("LOL estoy aca 222\n");
//
//        headList = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
//        headList->idProcess = procID;
//        headList->nextProcess = NULL;
//        headList->semaphoreName = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
//        sprintf(headList->semaphoreName, "%d", procID);
//        headList->semaphore = sem_open(headList->semaphoreName, O_CREAT, 0600, 0);
//        headList->inExecution = false;
//
//    } else {
//        printf("LOL estoy aca\n");
//        struct process *pProcess = headList;
//        while (pProcess->nextProcess != NULL) {
//            pProcess = headList->nextProcess;
//        }
//        pProcess->nextProcess = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
//        pProcess->nextProcess->idProcess = procID;
//        pProcess->nextProcess->nextProcess = NULL;
//        pProcess->nextProcess->semaphoreName = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
//        sprintf(pProcess->nextProcess->semaphoreName, "%d", procID);
//        pProcess->nextProcess->semaphore = sem_open(headList->semaphoreName, O_CREAT, 0600, 0);
//        pProcess->nextProcess->inExecution = false;
//    }
//    printf("Prueba\n");
    sem_post(proc);
    execute_process(process);
}

void execute_process(struct process* process){
    while (1==1){
        sem_wait(process->semaphore);
//        printf("Fui liberado, el proceso numero %d en la posicion de lista %d \n",getpid(),);

        process->inExecution = false;
    }
}