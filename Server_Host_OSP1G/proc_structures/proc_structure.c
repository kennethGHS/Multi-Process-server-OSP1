//
// Created by kenneth on 17/11/20.
//
#include "proc_structure.h"

/**
 * This method needs to be called to create all the processes, it inits the list of *numProcesses processes in a list
 * in shared memory, this processes can be accessed by the parend and the other processes
 * it also initiates the semaphore used for the administration of the list
 */

void initList(int processes) {

    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    numProcesses = mmap(NULL, sizeof(int), protection, visibility, -1, 0);
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
    headList->idProcess = -1;
    sem_init(headList->semaphore, 1, 0);
    struct process *initial = headList;
    for (int i = 0; i < *numProcesses + 1; ++i) {
        initial->nextProcess = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
        initial->nextProcess->available = true;
        initial->nextProcess->semaphore = mmap(NULL, sizeof(sem_t), protection, visibility, -1, 0);
        initial->nextProcess->semaphoreName = mmap(NULL, sizeof(struct process), protection, visibility, -1, 0);
        sem_init(initial->nextProcess->semaphore, 1, 0);
        initial->idProcess = -1;
        initial = initial->nextProcess;
    }
}
/**
 * Prints all processes ID
 */
void print_all_PID() {
    struct process *list = headList;
    while (list->available == false) {
        list = list->nextProcess;
    }
}
/**
 * Adds a new process to the list and begins its execution
 * @param procID
 */
void create_and_execute(int procID) {
    sem_t *proc = semaphoreList;
    int procValue;
    sem_getvalue(proc, &procValue);
    sem_wait(proc);
    struct process *process = headList;
    while (!process->available) {

        process = process->nextProcess;
    }
    process->idProcess = procID;
    sprintf(headList->semaphoreName, "%d", procID);
    process->inExecution = false;
    process->available = false;

    sem_post(proc);
    execute_process(process);
}
/**
 * The process a process goes through for its execution
 * @param process
 */
void execute_process(struct process *process) {

    while (1) {
        char *hello = "Hello from server";
        char buffer[1024] = {0};
        long int valread;
        sem_wait(process->semaphore);
        process->socket = receive();
        receive_picture(process->socket);
        close(process->socket);
        process->inExecution = false;
    }
}
/**
 * Releases a process in the list using its id
 * @param PID
 */
void release_by_ID(int PID) {
    struct process *head = headList;
    int i = 0;
    while (head->idProcess != PID && i != *numProcesses) {
        i++;
        head = head->nextProcess;
    }
    if (i == *numProcesses) {
        return;
    }
    int procValue;
    sem_getvalue(head->semaphore, &procValue);
    sem_post(head->semaphore);
}
/**
 * releases a proces and sets it available
 * @param socket
 * @return
 */
int release_and_set_available(int socket) {
    struct process *process = headList;
    int i = 0;
    while (process->inExecution && i != *numProcesses) {
        i++;
        process = process->nextProcess;
    }
    if (i == *numProcesses) {
        return -1;
    }
    process->socket = socket;
    process->inExecution = true;
    sem_post(process->semaphore);
    return 0;
}
/**
 * gets position of a process in the list using its id
 * @param PID
 * @return
 */
int get_position_list_PID(int PID) {
    struct process *head = headList;
    int i = 0;
    while (head->idProcess != PID && i != *numProcesses) {
        i++;
        head = head->nextProcess;
    }
    return i;
}
/**
 * Releases all processes in the list
 * @return
 */
int init_free_process() {
    struct process *reference = headList;
    while (reference->idProcess != -1) {
        printf("Closing process with id %d \n", reference->idProcess);
        kill(reference->idProcess, SIGKILL);
        reference->inExecution = false;
        reference->available = false;
        reference = reference->nextProcess;
    }
}
