//
// Created by kenneth on 17/11/20.
//

#ifndef SERVER_HOST_OSP1G_PROC_STRUCTURE_H
#define SERVER_HOST_OSP1G_PROC_STRUCTURE_H

#include <semaphore.h>
#include <z3.h>

struct process{
    struct process * nextProcess;
    sem_t * semaphore;
    int socket;
    bool inExecution;
    int idProcess;
    char * semaphoreName;
    bool available;
};

int * numProcesses;
sem_t * semaphoreList;
sem_t * semaphoreDelegation;
struct sockaddr_in * address;
//char *semaphoreListName ;
static struct process * headList;
void initList(int processes);
void print_all_PID();
void create_and_execute(int PID);
void release_by_ID(int PID);
int release_and_set_available(int socket);
int get_position_list_PID(int PID);
void execute_process(struct process* process); // se llama despues de crear, para esperar a la señal de inicio
int init_free_process();
bool is_process_free();



#endif //SERVER_HOST_OSP1G_PROC_STRUCTURE_H
